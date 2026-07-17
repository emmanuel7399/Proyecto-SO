# Proyecto-SO
# Proyecto final: Sistemas Operativos - Modificación de xv6

## Resumen
Este repositorio contiene el código fuente modificado de xv6, un sistema operativo académico basado en Unix. El objetivo de este proyecto es explorar, analizar y proponer mejoras en dos componentes fundamentales del sistema:
1. El algoritmo de planificación de la CPU (scheduler).
2. El algoritmo de asignación y gestión de memoria.

## Entorno de Desarrollo y Virtualización
Para garantizar que el sistema operativo se compila y ejecuta sin errores en cualquier máquina, el entorno de desarrollo se ha estandarizado utilizando **Docker**.

**Características del Entorno:**
* **Sistema Host:** Windows (o cualquier OS compatible con Docker).
* **Entorno de Contenedor:** Ubuntu 24.04 (Seleccionado específicamente para cumplir con la dependencia de QEMU >= 7.2 requerida por las versiones recientes de xv6).
* **Arquitectura:** RISC-V.
* **Herramientas instaladas:**
  * Compiladores cruzados (`gcc-riscv64-linux-gnu`, `binutils-riscv64-linux-gnu`, `build-essential`).
  * Emulador `qemu-system-misc`.
  * `bc` (Herramienta de cálculo requerida por el Makefile para la verificación de memoria).
  * `gdb-multiarch` para depuración.

## Pasos de Configuración Inicial aplicados

A continuación, se detallan las instrucciones para compilar y ejecutar la versión original (y posteriormente la modificada) de xv6:

### 1. Clonar el repositorio
Se obtuvo el código fuente base mediante:
\`\`\`bash
git clone https://github.com/mit-pdos/xv6-riscv.git
\`\`\`

### 2. Configurar la imagen de Docker
Se creó un archivo `Dockerfile` en la raíz del proyecto para instalar las dependencias exactas:
\`\`\`dockerfile
FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    git build-essential gdb-multiarch qemu-system-misc \
    gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu bc \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /xv6
CMD ["/bin/bash"]
\`\`\`

### 3. Construir la imagen
Para construir el entorno estandarizado, ejecutar:
\`\`\`bash
docker build -t entorno-xv6 .
\`\`\`

### 4. Iniciar el entorno y compilar xv6
Para ejecutar el contenedor montando el código local como un volumen (permitiendo usar IDEs como IntelliJ de forma nativa en el host), se utiliza:
\`\`\`bash
docker run -it -v "${PWD}:/xv6" entorno-xv6
\`\`\`

Una vez dentro del contenedor, para realizar una compilación limpia y ejecutar el sistema operativo, se ejecutan los siguientes comandos:
\`\`\`bash
make clean
make CPUS=1 qemu
\`\`\`
*(Nota: Se utiliza `CPUS=1` para forzar a QEMU a usar un solo núcleo durante la emulación, evitando así bloqueos o cuelgues durante el arranque en ciertas configuraciones de host).*

### 5. Salir del emulador
Para salir del entorno interactivo de xv6 de forma segura, presionar la combinación de teclas:
`Ctrl + A`, soltar y luego presionar `X`.

---
*Este documento se irá actualizando a medida que se implementen y prueben las modificaciones en los algoritmos de planificación y memoria.*
