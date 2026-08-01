Installation & Getting Started
===============================

This guide covers getting started with Gorgonzola, hosting local extension servers, and building the project from source.

Getting Started
---------------

Gorgonzola is embeddable and serverless. You can link it directly into your application binary without needing a separate database server process.

To start using Gorgonzola:

1. Clone the repository:

   .. code-block:: bash

      git clone https://github.com/gorgonzola-graph-db/gorgonzola.git
      cd gorgonzola

2. Refer to the build instructions below or explore sample queries and usage patterns in the ``examples/`` directory.

Extensions
----------

Gorgonzola supports runtime-loadable extensions to expand functionality (such as vector search, full-text search, and JSON support).

Pre-installed Extensions (v0.11.3+)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In Gorgonzola v0.11.3 and newer, the following core extensions are pre-installed by default (no ``INSTALL`` query command required):

* ``algo`` - Graph algorithm library
* ``fts`` - Full-Text Search indexing
* ``json`` - JSON data manipulation and query functions
* ``vector`` - HNSW vector search indexing

Hosting a Local Extension Server
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For custom builds or older versions requiring external extension downloads, you can host a local extension server using the provided NGINX Docker container:

1. Build and run the extension server container:

   .. code-block:: bash

      docker build -t gorgonzola-extension-server -f build-context/Dockerfile .
      docker run -d -p 8080:80 gorgonzola-extension-server

2. Once the server is running, install extensions by issuing Cypher queries pointing to your local endpoint:

   .. code-block:: cypher

      INSTALL <EXTENSION_NAME> FROM 'http://localhost:8080/';

Building from Source
--------------------

Prerequisites
~~~~~~~~~~~~~

* **C++ Compiler**: A C++20 compliant compiler (GCC 11+, Clang 13+, or MSVC 2022+).
* **Build System**: CMake 3.22 or higher and Ninja/Make.
* **Python**: Python 3.9+ (for utility scripts and testing).

Build Instructions
~~~~~~~~~~~~~~~~~~

To build Gorgonzola from source using CMake:

.. code-block:: bash

   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make -j$(nproc)
