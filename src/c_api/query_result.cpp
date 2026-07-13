#include "common/types/types.h"
#include "main/query_result.h"

#include "c_api/helpers.h"
#include "c_api/gorgonzola.h"
#include "c_api_utils.h"

using namespace gorgonzola::main;
using namespace gorgonzola::common;
using namespace gorgonzola::processor;

void gorgonzola_query_result_destroy(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    if (query_result == nullptr) {
        return;
    }
    if (query_result->_query_result != nullptr) {
        if (!query_result->_is_owned_by_cpp) {
            gorgonzola::c_api::HandleRegistry::getInstance().unregisterHandle(query_result->_query_result);
        delete static_cast<QueryResult*>(query_result->_query_result);
        }
    }

    } catch (...) {
        gorgonzola::c_api::translate_exception();
    }
}

bool gorgonzola_query_result_is_success(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    return static_cast<QueryResult*>(query_result->_query_result)->isSuccess();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

char* gorgonzola_query_result_get_error_message(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    auto error_message = static_cast<QueryResult*>(query_result->_query_result)->getErrorMessage();
    if (error_message.empty()) {
        return nullptr;
    }
    return convertToOwnedCString(error_message);

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

uint64_t gorgonzola_query_result_get_num_columns(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    return static_cast<QueryResult*>(query_result->_query_result)->getNumColumns();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return 0;
    }
}

gorgonzola_state gorgonzola_query_result_get_column_name(gorgonzola_query_result* query_result, uint64_t index,
    char** out_column_name) {
        GORGONZOLA_C_API_BEGIN

    auto column_names = static_cast<QueryResult*>(query_result->_query_result)->getColumnNames();
    if (index >= column_names.size()) {
        return GorgonzolaError;
    }
    *out_column_name = convertToOwnedCString(column_names[index]);
    return GorgonzolaSuccess;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_query_result_get_column_data_type(gorgonzola_query_result* query_result, uint64_t index,
    gorgonzola_logical_type* out_column_data_type) {
        GORGONZOLA_C_API_BEGIN

    auto column_data_types =
        static_cast<QueryResult*>(query_result->_query_result)->getColumnDataTypes();
    if (index >= column_data_types.size()) {
        return GorgonzolaError;
    }
    const auto& column_data_type = column_data_types[index];
    out_column_data_type->_data_type = new LogicalType(column_data_type.copy());
    return GorgonzolaSuccess;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}

uint64_t gorgonzola_query_result_get_num_tuples(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    return static_cast<QueryResult*>(query_result->_query_result)->getNumTuples();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return 0;
    }
}

gorgonzola_state gorgonzola_query_result_get_query_summary(gorgonzola_query_result* query_result,
    gorgonzola_query_summary* out_query_summary) {
        GORGONZOLA_C_API_BEGIN

    if (out_query_summary == nullptr) {
        return GorgonzolaError;
    }
    auto query_summary = static_cast<QueryResult*>(query_result->_query_result)->getQuerySummary();
    out_query_summary->_query_summary = query_summary;
        gorgonzola::c_api::HandleRegistry::getInstance().registerHandle(query_summary, gorgonzola::c_api::HandleType::QuerySummary);
    return GorgonzolaSuccess;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}

bool gorgonzola_query_result_has_next(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    return static_cast<QueryResult*>(query_result->_query_result)->hasNext();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

bool gorgonzola_query_result_has_next_query_result(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    return static_cast<QueryResult*>(query_result->_query_result)->hasNextQueryResult();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return false;
    }
}

gorgonzola_state gorgonzola_query_result_get_next_query_result(gorgonzola_query_result* query_result,
    gorgonzola_query_result* out_query_result) {
        GORGONZOLA_C_API_BEGIN

    if (!gorgonzola_query_result_has_next_query_result(query_result)) {
        return GorgonzolaError;
    }
    auto next_query_result =
        static_cast<QueryResult*>(query_result->_query_result)->getNextQueryResult();
    if (next_query_result == nullptr) {
        return GorgonzolaError;
    }
    out_query_result->_query_result = next_query_result;
    out_query_result->_is_owned_by_cpp = true;
    return GorgonzolaSuccess;

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_query_result_get_next(gorgonzola_query_result* query_result,
    gorgonzola_flat_tuple* out_flat_tuple) {
        GORGONZOLA_C_API_BEGIN
        auto flat_tuple = static_cast<QueryResult*>(query_result->_query_result)->getNext();
        out_flat_tuple->_flat_tuple = flat_tuple.get();
        out_flat_tuple->_is_owned_by_cpp = true;
        return GorgonzolaSuccess;
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}

char* gorgonzola_query_result_to_string(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    auto* qr = static_cast<QueryResult*>(query_result->_query_result);
    auto savedIterPos = qr->getNextTupleIdx();
    std::string result_string = qr->toString();
    qr->resetIteratorTo(savedIterPos);
    return convertToOwnedCString(result_string);

    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return nullptr;
    }
}

void gorgonzola_query_result_reset_iterator(gorgonzola_query_result* query_result) {
        GORGONZOLA_C_API_BEGIN

    static_cast<QueryResult*>(query_result->_query_result)->resetIterator();

    } catch (...) {
        gorgonzola::c_api::translate_exception();
    }
}

#ifndef GORGONZOLA_LITE
gorgonzola_state gorgonzola_query_result_get_arrow_schema(gorgonzola_query_result* query_result,
    ArrowSchema* out_schema) {
        GORGONZOLA_C_API_BEGIN
        *out_schema = *static_cast<QueryResult*>(query_result->_query_result)->getArrowSchema();
        return GorgonzolaSuccess;
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}

gorgonzola_state gorgonzola_query_result_get_next_arrow_chunk(gorgonzola_query_result* query_result,
    int64_t chunk_size, ArrowArray* out_arrow_array) {
        GORGONZOLA_C_API_BEGIN
        *out_arrow_array =
            *static_cast<QueryResult*>(query_result->_query_result)->getNextArrowChunk(chunk_size);
        return GorgonzolaSuccess;
    } catch (...) {
        gorgonzola::c_api::translate_exception();
        return GorgonzolaError;
    }
}
#endif
