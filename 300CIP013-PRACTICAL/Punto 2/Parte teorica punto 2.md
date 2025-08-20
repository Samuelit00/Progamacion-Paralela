# Punto 2

## 2.1. `printf` vs `write` y redirección
### Programa 1
**Archivo:** `src/1_printf_write.c`  
**Comportamiento:** `printf()` usa **buffering** de stdio, mientras que `write()` escribe sin buffer directamente al descriptor de archivo 1 (stdout).

- **Interactivamente (en terminal)**: `stdout` suele ser **line-buffered**. Como `printf("...\n")` termina en salto de línea, el buffer se **vacía antes** de llamar a `write()`.  
  **Salida esperada (terminal):**
  ```
  Line 1 ..
  Line 2 
  ```

- **Redirigiendo a archivo**: `stdout` pasa a ser **fully-buffered**. El `\n` **no garantiza** el vaciado inmediato; `write()` (no buffer) puede llegar al archivo **antes** que el contenido buffered de `printf`.  
  **Salida típica (archivo):**
  ```
  Line 2 Line 1 ..
  ```

### Programa 2 (con `fflush`)
**Archivo:** `src/2_printf_fflush_write.c`  
Se añade `fflush(stdout)`. Así obligamos a vaciar el buffer de `printf` **antes** de `write()`.  
**Orden estable tanto en terminal como en archivo:**
```
Line 1 ..
Line 2 
```

> **Idea clave:** `stdio` (printf) y `write` comparten destino (fd=1) pero **no** el mismo buffer. Controlar cuándo se vacía `stdio` evita cambios de orden al redirigir.

## 2.2. `fork()` y orden de ejecución
### Programa 3 (sin `wait()`)
**Archivo:** `src/3_fork_no_wait.c`  
Cada llamada a `fork()` duplica el proceso que ejecuta el bucle; la **planificación del SO** puede intercalar salidas de padres e hijos en **orden variable** cada ejecución. Esto ilustra **concurrencia** y **no determinismo** en el orden de impresión.

### Programa 4 (con `wait()`)
**Archivo:** `src/4_fork_wait.c`  
El padre llama a `wait(NULL)` tras cada `fork()`, forzando a que el **hijo imprima primero** y **luego** el padre para esa iteración. El orden global sigue dependiendo del scheduler, pero por iteración se respeta *hijo → padre*.
- Nota: el hijo hace `return 0;` para **no** seguir forkeando en iteraciones siguientes.

## 2.3. `encrypt_it` / `decrypt_it` (C, C++ y Bash)
Cada programa/Script recibe **dos argumentos**: el **índice de rotación** y la **frase** (puede llevar espacios).  
> **Soporte de caracteres:** Solo se rotan letras ASCII `[A–Z][a–z]`. Caracteres con acento (á, é, í, ó, ú, ñ) y símbolos **se preservan** tal cual.

### Uso (C/C++)
```bash
./bin/encrypt_it_c 7 "Aprendiendo Programación paralela"
./bin/decrypt_it_c 7 "Zú iolxgju Ikygx, latioutg!"

./bin/encrypt_it_cpp 3 "Aprendiendo Programación paralela"
./bin/decrypt_it_cpp 7 "Zú iolxgju Ikygx, latioutg!"
```

### Uso (Bash)
```bash
./scripts/encrypt_it.sh 7 "Aprendiendo Programación paralela"
./scripts/decrypt_it.sh 7 "Zú iolxgju Ikygx, latioutg!"
```

### Redirección a archivos (requisito)
```bash
./bin/encrypt_it_c 7 "Aprendiendo Programación paralela" > samples/encrypt_k7.txt
./bin/encrypt_it_cpp 3 "Aprendiendo Programación paralela" > samples/encrypt_k3.txt
cat samples/encrypt_k7.txt
cat samples/encrypt_k3.txt

./bin/decrypt_it_c 7 "Zú iolxgju Ikygx, latioutg!" > samples/decrypt_rot7.txt
cat samples/decrypt_rot7.txt
```

### Salidas de ejemplo (precalculadas)
- **encrypt (k=7)**: `Hwylukplukv Wyvnyhthjpóu whyhslsh`
- **encrypt (k=3)**: `Dsuhqglhqgr Surjudpdflóq sdudohod`
- **decrypt rot 7** de `"Zú iolxgju Ikygx, latioutg!"`: `Sú bheqzcn Bdrzq, etmbhnmz!`

## Preguntas guía
- **¿Se alcanzó código paralelo o comportamiento paralelo?**  
  En los ejercicios de `fork()`, hay **concurrencia** real (múltiples procesos potencialmente en paralelo según el SO y CPU). En `printf/write` no hay paralelismo, solo efectos de **buffering**.
- **¿El shell hace paralelismo o *pipelining*?**  
  El shell puede hacer ambos: `&` lanza procesos en **paralelo**; las tuberías `|` implementan **pipelining** (concurrencia encadenando stdin/stdout).
- **Si programar con scripts es tan fácil, ¿para qué más?**  
  Scripts son excelentes para *glue code* y orquestación, pero tienen límites en **rendimiento**, **control fino de memoria**, **tipado**, **portabilidad binaria** y **herramientas de depuración** frente a C/C++.
