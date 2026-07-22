# Security Policy

## Supported Versions

Gorgonzola is under active development. We provide security updates for the latest minor release series. 

| Version | Supported          |
| ------- | ------------------ |
| latest  | ✅ Yes              |
| < 1.0   | ❌ No (Pre-release) |

*Note: As the project approaches a v1.0 release, we will publish a formal Long-Term Support (LTS) schedule.*

## Reporting a Vulnerability

The Gorgonzola team takes security bugs seriously. We appreciate your efforts to responsibly disclose your findings, and will make every effort to acknowledge your contributions.

**Please do not report security vulnerabilities through public GitHub issues.**

Instead, please report them using **GitHub's Private Vulnerability Reporting** feature:
1. Go to the [Security tab](https://github.com/gorgonzola-graph-db/gorgonzola/security) of the main repository.
2. Click **"Report a vulnerability"**.

Alternatively, you may email the maintainers directly at: `security@gorgonzola-graph-db.org` (Replace with actual email if different).

### What to Include in Your Report
To help us triage and fix the issue quickly, please include:
*   The exact version of Gorgonzola you are using (commit hash or release tag).
*   The operating system and architecture (e.g., Ubuntu 22.04 x86_64, Windows 11 ARM).
*   The specific API or submodule involved (e.g., Core C++, Python API, httpfs extension).
*   Step-by-step instructions to reproduce the issue.
*   A minimal proof of concept (PoC) or exploit code.
*   The potential impact of the vulnerability.

### Response Timeline
*   **Acknowledgment:** We will acknowledge receipt of your report within **48 hours**.
*   **Assessment:** We will provide an initial assessment of the report and confirm whether it is accepted as a security vulnerability within **5 business days**.
*   **Updates:** We will keep you informed of our progress at least once every 7 days until the issue is resolved or mitigated.
*   **Disclosure:** Once a fix is released, we will publicly disclose the vulnerability and credit you (unless you prefer to remain anonymous).

## Scope

### In Scope
The following components within this repository are actively maintained and monitored for security issues:
*   **Core Database Engine (`src/`)**: The C++ query parser, planner, optimizer, and execution engine.
*   **Storage Layer**: Buffer manager, transaction manager, and on-disk file formats.
*   **API Bindings (`modules/gorgonzola-api-langs/`)**: Python, Rust, Node.js, and Java APIs (including FFI boundaries like JNI and pybind11).
*   **Native Extensions (`modules/gorgonzola-db-extensions/`)**: Built-in extensions including `httpfs`, `vector` (HNSW), `json`, `fts` (Full-Text Search), `postgres`, `sqlite`, and `iceberg`.

### Out of Scope
*   **Third-Party Libraries (`modules/third_party/`)**: We vendor libraries such as `pybind11`, `antlr4_runtime`, `brotli`, `re2`, and `zstd`. Vulnerabilities in these libraries should be reported directly to their upstream maintainers. We monitor upstream security advisories and will update our vendored copies accordingly.
*   **Denial of Service via Resource Exhaustion (Local)**: Because Gorgonzola is an embedded database, running queries locally with insufficient hardware will naturally lead to out-of-memory (OOM) or CPU exhaustion. We are actively working on hardening resource limits (see buffer manager and hash index TODOs), but local OOM on untrusted local workloads is generally not considered a security vulnerability for embedded databases.
*   **Physical Attacks**: Attacks requiring physical access to the disk/memory of the host machine.

## Security Considerations & Best Practices

Because Gorgonzola is an embedded database (similar to SQLite), security is a shared responsibility between the database engine and the host application. 

### 1. Query Injection
Even though Gorgonzola uses a parameterized query parser (Cypher), you should **never** concatenate user input into query strings. Always use parameterized queries or prepared statements provided by your language's API (e.g., `conn.execute("MATCH (n) WHERE n.name = $name RETURN n", {"name": user_input})`).

### 2. File System Access
The database engine runs with the exact file system permissions of the host process. 
*   The `COPY FROM` and `COPY TO` commands can read/write files. Ensure the host process has the principle of least privilege applied.
*   Be cautious when using the `httpfs` extension, as it allows the database to make outbound HTTP/S3 requests to arbitrary URLs based on query input.

### 3. Memory Safety (FFI Boundaries)
The core of Gorgonzola is written in C++, which requires manual memory management. We actively audit our Foreign Function Interface (FFI) boundaries (e.g., JNI for Java, pybind11 for Python, `cdylib` for Rust) to prevent memory leaks, use-after-frees, and integer overflows when passing data across language boundaries.

## Disclosure Policy

When a security vulnerability is confirmed and a patch is ready:
1. We will prepare a patch and merge it into the main branch.
2. We will cut a new release containing the patch as soon as possible.
3. We will publish a GitHub Security Advisory detailing the vulnerability, its impact, and the steps to upgrade.
4. We will request a CVE identifier if the vulnerability is of high or critical severity.
