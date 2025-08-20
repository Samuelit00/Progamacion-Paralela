# Punto 3

## Explicación de los Programas

### **1. sum.c**
- Crea un proceso hijo con `fork()`.
- El hijo calcula la **suma** de un rango y envía el resultado al padre usando **pipes**.
- El padre imprime el resultado final.

### **2. adder.c**
- Divide el cálculo entre **n/3** procesos hijos.
- Cada hijo calcula una suma parcial.
- El padre combina los resultados.

### **3. randomizer.c**
- Genera **n** números aleatorios.
- Puede usarse en **pipeline** con `adder.c`:

### **4. matrix_threads.c**
- Usa **pthread** para crear **2 hilos**:
  - Uno suma matrices.
  - Otro calcula la diferencia.
- El hilo principal multiplica los resultados.
- Se mide el tiempo de ejecución.

### **5. matrix_no_threads.c**
- Realiza las mismas operaciones que `matrix_threads.c`, pero de forma **secuencial**.
- Permite comparar tiempos.

### **6. producer_consumer.c**
- Simula el clásico problema **productor-consumidor**.
- Usa **mutex** y **semaphores** para evitar conflictos.
- Permite múltiples productores y consumidores.

---

## Respuestas a las Preguntas

### **A. Procesos vs Threads**
| Aspecto         | Procesos (`fork`)       | Threads (`pthread`)     |
|----------------|------------------------|-------------------------|
| **Memoria**    | No comparten memoria. Cada proceso tiene su propio espacio. | Comparten **memoria global** y **heap**. |
| **Velocidad**  | Creación más lenta.     | Creación rápida. |
| **Comunicación** | Necesitan pipes, sockets o shared memory. | Acceden a las mismas variables globales. |
| **Seguridad**  | Más seguros, no interfieren entre sí. | Riesgo de condiciones de carrera. |

---

### **B. ¿Los procesos hijos corren en paralelo o concurrentemente?**
- Si tu CPU tiene **un solo núcleo**, los procesos corren **concurrentemente** (el sistema operativo alterna entre ellos).
- Si tu CPU tiene **múltiples núcleos**, pueden correr **en paralelo real**.

---

### **C. Diferencia de tiempos entre `sum.c` y `adder.c`**
- `sum.c` usa **un solo proceso hijo** → más lento para listas grandes.
- `adder.c` divide el cálculo entre varios procesos → generalmente más rápido.

---

### **D. ¿Padre e hijos comparten memoria?**
- **No**. Cada proceso tiene su propio espacio de direcciones.
- Se comunican mediante **pipes** o **memoria compartida**.

---

### **E. ¿Qué implementación es más rápida: threads o procesos?**
- En la mayoría de los casos, **threads** (`pthread`) son más rápidos:
    - No necesitan duplicar la memoria del proceso.
    - No requieren pipes ni IPC para comunicarse.

---

### **F. ¿Los threads siempre son más rápidos?**
- **No siempre**.  
Si la tarea es muy pequeña, el **overhead** de crear y sincronizar hilos puede hacerlos más lentos.

---

### **G. ¿Qué memoria comparten los threads?**
- **Sí comparten**:
    - Segmento de **datos globales**.
    - **Heap**.
- **No comparten**:
    - Cada hilo tiene su **propia pila** (stack).

---

### **H. Problema productor-consumidor**
- **¿Funciona con más productores o consumidores?**  
  Sí, mientras se usen correctamente **mutex** y **semaphores**.
- **¿Qué pasa si no usamos sincronización?**  
  Aparecen **condiciones de carrera**: dos hilos podrían escribir o leer el mismo dato al mismo tiempo.
- **¿Por qué usamos semáforos?**  
  Para coordinar la cantidad de elementos en el buffer y evitar desbordamientos.

---

## 📊 Comparativa de Rendimiento

| Programa                 | Enfoque         | Tiempo estimado |
|------------------------|------------------|-----------------|
| `sum.c`                | 1 proceso       | Lento |
| `adder.c`              | Varios procesos | Medio |
| `matrix_no_threads.c`  | Secuencial      | Lento |
| `matrix_threads.c`     | Threads         | Rápido |
| `producer_consumer.c`  | Threads + Semáforos | Depende de la carga |
