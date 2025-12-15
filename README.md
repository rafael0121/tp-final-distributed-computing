# tp-final-distributed-computing

Guia de execução do projeto de computação distribuída com gRPC/Protobuf.

**Pré-requisitos**
- **CMake** 3.13+
- **C++17** toolchain (g++, clang ou equivalente)
- **gRPC** e **Protocol Buffers** instalados e disponíveis via `find_package`
	- Em distros baseadas em Debian/Ubuntu, uma forma comum é usar pacotes/compilação do gRPC.
	- Alternativamente, use `vcpkg` ou `conan` para instalar `grpc` e `protobuf`.

**Estrutura de build**
- `setup.sh`: gera a pasta `build/` e configura o CMake.
- `run.sh`: compila o projeto (modo Debug) dentro de `build/`.
- Executável gerado: `build/node/node`.

**Configurar e compilar**
```bash
./setup.sh
./run.sh
```

Se preferir manualmente:
```bash
mkdir -p build
cd build
cmake ..
cmake --build . --config Debug -j
```

**Como executar**
O binário aceita parâmetros:
- `--address <ip:port>`: endereço onde o servidor gRPC do nó irá escutar. Obrigatório.
- `--knownode <ip:port>`: endereço de um nó já existente para sincronizar estado inicial. Opcional.

Exemplos de execução em uma única máquina (ports diferentes):

1) Iniciar o primeiro nó (vira coordenador inicial):
```bash
./build/node/node --address 127.0.0.1:50051
```

2) Iniciar um segundo nó e sincronizar com o primeiro:
```bash
./build/node/node --address 127.0.0.1:50052 --knownode 127.0.0.1:50051
```

3) Iniciar um terceiro nó apontando para qualquer nó vivo (idealmente o coordenador):
```bash
./build/node/node --address 127.0.0.1:50053 --knownode 127.0.0.1:50051
```

Cada nó inicia três threads:
- Servidor gRPC (aceita chamadas dos serviços de descoberta e eleição).
- Loop principal: sincroniza com o coordenador e inicia eleição se ele falhar.
- Impressão periódica de status (a cada 10s).

**Fluxo resumido**
- Sem `--knownode`: o nó define-se como coordenador inicial.
- Com `--knownode`: o nó sincroniza listas de nós/sensores e líder via RPC `Hello`.
- Se a sincronização com o coordenador falhar, é iniciada uma eleição em anel.