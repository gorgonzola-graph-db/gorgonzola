# Embeddability Matrix (C++/C) from Stage 0 Audit

| Subsystem | Already Embeddable | Changes Needed | Difficulty | C++ Specific Issues |
|-----------|-------------------|----------------|------------|---------------------|
| Subsystem | Already Embeddable | Changes Needed | Difficulty | C++ Specific Issues |
|-----------|-------------------|----------------|------------|---------------------|
| Parser | ⚠️ | Resolve: Global state, Exceptions used, Raw pointers | Medium | Global state, Exceptions used, Raw pointers |
| Binder | ⚠️ | Resolve: Global state, Exceptions used, Raw pointers | Medium | Global state, Exceptions used, Raw pointers |
| Optimizer | ⚠️ | Resolve: Global state, Exceptions used | Medium | Global state, Exceptions used |
| Executor | ❓ | Directory not found | Unknown | N/A |
| Storage | ⚠️ | Resolve: Global state, Exceptions used, Raw pointers | Medium | Global state, Exceptions used, Raw pointers |
| Catalog | ⚠️ | Resolve: Global state, Exceptions used, Raw pointers | Medium | Global state, Exceptions used, Raw pointers |
| Transaction | ⚠️ | Resolve: Exceptions used | Medium | Exceptions used |
| Extension _manager | ❓ | Directory not found | Unknown | N/A |
| C _api | ❓ | Directory not found | Unknown | N/A |
