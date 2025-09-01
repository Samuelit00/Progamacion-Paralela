#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "image.h"  // tus headers

using namespace std;

// Saltar comentarios en archivos PGM/PPM ASCII
static void skipComments(istream &in) {
    int c;
    while ((c = in.peek()) != EOF) {
        if (c == '#') {
            string dummy; getline(in, dummy);
        } else if (isspace(c)) {
            in.get();
        } else break;
    }
}

// Leer imagen a buffer
static bool readImageToBuffer(const char* path, int &typecode,
                              int &W, int &H, int &maxVal,
                              int* &buffer, int &count)
{
    ifstream in(path);
    if (!in.is_open()) {
        cerr << "Error: no se pudo abrir " << path << "\n";
        return false;
    }

    string magic;
    in >> magic;
    if (!(magic == "P2" || magic == "P3")) {
        cerr << "Formato no soportado: " << magic << "\n";
        return false;
    }
    typecode = (magic == "P3") ? 1 : 0;

    skipComments(in);
    in >> W >> H;
    skipComments(in);
    in >> maxVal;

    if (typecode == 0) {
        count = W * H;
        buffer = new int[count];
        for (int i = 0; i < count; ++i) in >> buffer[i];
    } else {
        count = W * H * 3;
        buffer = new int[count];
        for (int i = 0; i < count; ++i) in >> buffer[i];
    }

    in.close();
    return true;
}

// Escribir PGM desde buffer
static void writePGM(const string &path, int W, int H, int maxVal, int* buf) {
    ofstream out(path);
    out << "P2\n" << W << " " << H << "\n" << maxVal << "\n";
    int idx = 0;
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            out << buf[idx++];
            if (x + 1 < W) out << " ";
        }
        out << "\n";
    }
}

// Escribir PPM desde buffer
static void writePPM(const string &path, int W, int H, int maxVal, int* buf) {
    ofstream out(path);
    out << "P3\n" << W << " " << H << "\n" << maxVal << "\n";
    int idx = 0;
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            out << buf[idx++] << " " << buf[idx++] << " " << buf[idx++];
            if (x + 1 < W) out << " ";
        }
        out << "\n";
    }
}

// Worker: aplanar PGM desde objeto PGMImage
static int* flattenPGM(PGMImage* img, int W, int H) {
    int* out = new int[W*H];
    int k=0;
    for (int y=0;y<H;y++)
        for (int x=0;x<W;x++)
            out[k++] = img->pixels[y][x];
    return out;
}

// Worker: aplanar PPM desde objeto PPMImage
static int* flattenPPM(PPMImage* img, int W, int H) {
    int* out = new int[W*H*3];
    int k=0;
    for (int y=0;y<H;y++)
        for (int x=0;x<W;x++) {
            out[k++] = img->pixels[y][x].r;
            out[k++] = img->pixels[y][x].g;
            out[k++] = img->pixels[y][x].b;
        }
    return out;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0, size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 3) {
        if (rank == 0) cerr << "Uso: mpirun -np 4 ./mpi_run <input> <output_prefix>\n";
        MPI_Finalize();
        return 1;
    }

    // --- MAESTRO ---
    if (rank == 0) {
        const char* infile = argv[1];
        const char* outprefix = argv[2];

        int typecode, W, H, maxVal, count;
        int* buffer = nullptr;
        if (!readImageToBuffer(infile, typecode, W, H, maxVal, buffer, count)) {
            MPI_Abort(MPI_COMM_WORLD, 2);
        }

        int header[5] = { typecode, W, H, maxVal, count };
        double t0 = MPI_Wtime();

        // Enviar a workers
        for (int dest=1; dest<=3; ++dest) {
            MPI_Send(header, 5, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(buffer, count, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }

        // Recibir resultados
        for (int i=0; i<3; ++i) {
            MPI_Status st;
            int rheader[5];
            MPI_Recv(rheader, 5, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &st);

            int src = st.MPI_SOURCE;
            int rtype = rheader[0], rW = rheader[1], rH = rheader[2], rMax = rheader[3], rCount = rheader[4];

            int* rbuf = new int[rCount];
            MPI_Recv(rbuf, rCount, MPI_INT, src, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            string outname = string(outprefix);
            if (rtype == 0) {
                if (src == 1) outname += "_blur.pgm";
                else if (src == 2) outname += "_laplace.pgm";
                else outname += "_sharpening.pgm";
                writePGM(outname, rW, rH, rMax, rbuf);
            } else {
                if (src == 1) outname += "_blur.ppm";
                else if (src == 2) outname += "_laplace.ppm";
                else outname += "_sharpening.ppm";
                writePPM(outname, rW, rH, rMax, rbuf);
            }

            delete[] rbuf;
        }

        double t1 = MPI_Wtime();
        cout << "[MAESTRO] Tiempo total: " << (t1 - t0) << " segundos" << endl;
        delete[] buffer;
    }
   // --- WORKERS ---
    else {
        int header[5];
        MPI_Recv(header, 5, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int typecode = header[0], W = header[1], H = header[2], maxVal = header[3], count = header[4];

        int* buf = new int[count];
        MPI_Recv(buf, count, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        double t_start = MPI_Wtime();

        string tmpfile = string("/tmp/tmp_worker_") + to_string(rank) + (typecode == 0 ? ".pgm" : ".ppm");
        ofstream out(tmpfile);
        if (typecode == 0) {
            out << "P2\n" << W << " " << H << "\n" << maxVal << "\n";
            int k=0;
            for (int y=0;y<H;y++) {
                for (int x=0;x<W;x++) {
                    out << buf[k++];
                    if (x+1<W) out << " ";
                }
                out << "\n";
            }
        } else {
            out << "P3\n" << W << " " << H << "\n" << maxVal << "\n";
            int k=0;
            for (int y=0;y<H;y++) {
                for (int x=0;x<W;x++) {
                    out << buf[k++] << " " << buf[k++] << " " << buf[k++];
                    if (x+1<W) out << " ";
                }
                out << "\n";
            }
        }
        out.close();

        if (typecode == 0) {
            PGMImage img;
            freopen(tmpfile.c_str(), "r", stdin);
            img.readStdin();
            if (rank == 1) img.blur();
            else if (rank == 2) img.laplace();
            else img.sharpening();
            int* outbuf = flattenPGM(&img, W, H);
            int outHeader[5] = { 0, W, H, maxVal, W*H };
            MPI_Send(outHeader, 5, MPI_INT, 0, 2, MPI_COMM_WORLD);
            MPI_Send(outbuf, W*H, MPI_INT, 0, 3, MPI_COMM_WORLD);
            delete[] outbuf;
        } else {
            PPMImage img;
            freopen(tmpfile.c_str(), "r", stdin);
            img.readStdin();
            if (rank == 1) img.blur();
            else if (rank == 2) img.laplace();
            else img.sharpening();
            int* outbuf = flattenPPM(&img, W, H);
            int outHeader[5] = { 1, W, H, maxVal, W*H*3 };
            MPI_Send(outHeader, 5, MPI_INT, 0, 2, MPI_COMM_WORLD);
            MPI_Send(outbuf, W*H*3, MPI_INT, 0, 3, MPI_COMM_WORLD);
            delete[] outbuf;
        }

        double t_end = MPI_Wtime();
        cout << "[WORKER " << rank << "] Tiempo de ejecución: " << (t_end - t_start) << " segundos" << endl;

        remove(tmpfile.c_str());
        delete[] buf;
    }

    MPI_Finalize();
    return 0;
}
