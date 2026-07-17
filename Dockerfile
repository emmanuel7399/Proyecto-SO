# Cambiamos a Ubuntu 24.04 para asegurar QEMU >= 7.2
FROM ubuntu:24.04

# Evitar interacciones manuales durante la instalación
ENV DEBIAN_FRONTEND=noninteractive

# Instalar las herramientas de compilación para C, QEMU para RISC-V y bc
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    gdb-multiarch \
    qemu-system-misc \
    gcc-riscv64-linux-gnu \
    binutils-riscv64-linux-gnu \
    bc \
    && rm -rf /var/lib/apt/lists/*

# Establecer el directorio de trabajo
WORKDIR /xv6

# Abrir una terminal bash por defecto al iniciar
CMD ["/bin/bash"]
