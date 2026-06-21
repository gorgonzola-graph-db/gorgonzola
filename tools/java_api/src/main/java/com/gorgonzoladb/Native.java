package com.gorgonzoladb;

import java.util.Map;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

/**
 * Native is a wrapper class for the native library.
 * It is used to load the native library and call the native functions.
 * This class is not intended to be used by end users.
 */
public class Native {
    static {
        try {
            String os_name = "";
            String os_arch;
            String os_name_detect = System.getProperty("os.name").toLowerCase().trim();
            String os_arch_detect = System.getProperty("os.arch").toLowerCase().trim();
            boolean isAndroid = System.getProperty("java.runtime.name", "").toLowerCase().contains("android")
                || System.getProperty("java.vendor", "").toLowerCase().contains("android")
                || System.getProperty("java.vm.name", "").toLowerCase().contains("dalvik");
            switch (os_arch_detect) {
                case "x86_64":
                case "amd64":
                    os_arch = "amd64";
                    break;
                case "aarch64":
                case "arm64":
                    os_arch = "arm64";
                    break;
                case "i386":
                    os_arch = "i386";
                    break;
                default:
                    throw new IllegalStateException("Unsupported system architecture");
            }
            if (isAndroid){
                os_name = "android";
            }
            else if (os_name_detect.startsWith("windows")) {
                os_name = "windows";
            } else if (os_name_detect.startsWith("mac")) {
                os_name = "osx";
            } else if (os_name_detect.startsWith("linux")) {
                os_name = "linux";
            }
            String lib_res_name = "/libgorgonzola_java_native.so" + "_" + os_name + "_" + os_arch;

            Path lib_file = Files.createTempFile("libgorgonzola_java_native", ".so");
            URL lib_res = Native.class.getResource(lib_res_name);
            if (lib_res == null) {
                throw new IOException(lib_res_name + " not found");
            }
            Files.copy(lib_res.openStream(), lib_file, StandardCopyOption.REPLACE_EXISTING);
            new File(lib_file.toString()).deleteOnExit();
            String lib_path = lib_file.toAbsolutePath().toString();
            System.load(lib_path);
            if (os_name.equals("linux")) {
                gorgonzolaNativeReloadLibrary(lib_path);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Hack: Reload the native library again in JNI bindings to work around the
    // extension loading issue on Linux as System.load() does not set
    // `RTLD_GLOBAL` flag and there is no way to set it in Java.
    protected static native void gorgonzolaNativeReloadLibrary(String libPath);

    // Database
    protected static native long gorgonzolaDatabaseInit(String databasePath, long bufferPoolSize,
            boolean enableCompression, boolean readOnly, long maxDbSize, boolean autoCheckpoint,
            long checkpointThreshold,boolean throwOnWalReplayFailure, boolean enableChecksums);

    protected static native void gorgonzolaDatabaseDestroy(Database db);

    protected static native void gorgonzolaDatabaseSetLoggingLevel(String loggingLevel);

    // Connection
    protected static native long gorgonzolaConnectionInit(Database database);

    protected static native void gorgonzolaConnectionDestroy(Connection connection);

    protected static native void gorgonzolaConnectionSetMaxNumThreadForExec(
            Connection connection, long numThreads);

    protected static native long gorgonzolaConnectionGetMaxNumThreadForExec(Connection connection);

    protected static native QueryResult gorgonzolaConnectionQuery(Connection connection, String query);

    protected static native PreparedStatement gorgonzolaConnectionPrepare(
            Connection connection, String query);

    protected static native QueryResult gorgonzolaConnectionExecute(
            Connection connection, PreparedStatement preparedStatement, Map<String, Value> param);

    protected static native void gorgonzolaConnectionInterrupt(Connection connection);

    protected static native void gorgonzolaConnectionSetQueryTimeout(
            Connection connection, long timeoutInMs);

    // PreparedStatement
    protected static native void gorgonzolaPreparedStatementDestroy(PreparedStatement preparedStatement);

    protected static native boolean gorgonzolaPreparedStatementIsSuccess(PreparedStatement preparedStatement);

    protected static native String gorgonzolaPreparedStatementGetErrorMessage(
            PreparedStatement preparedStatement);

    // QueryResult
    protected static native void gorgonzolaQueryResultDestroy(QueryResult queryResult);

    protected static native boolean gorgonzolaQueryResultIsSuccess(QueryResult queryResult);

    protected static native String gorgonzolaQueryResultGetErrorMessage(QueryResult queryResult);

    protected static native long gorgonzolaQueryResultGetNumColumns(QueryResult queryResult);

    protected static native String gorgonzolaQueryResultGetColumnName(QueryResult queryResult, long index);

    protected static native DataType gorgonzolaQueryResultGetColumnDataType(
            QueryResult queryResult, long index);

    protected static native long gorgonzolaQueryResultGetNumTuples(QueryResult queryResult);

    protected static native QuerySummary gorgonzolaQueryResultGetQuerySummary(QueryResult queryResult);

    protected static native boolean gorgonzolaQueryResultHasNext(QueryResult queryResult);

    protected static native FlatTuple gorgonzolaQueryResultGetNext(QueryResult queryResult);

    protected static native boolean gorgonzolaQueryResultHasNextQueryResult(QueryResult queryResult);

    protected static native QueryResult gorgonzolaQueryResultGetNextQueryResult(QueryResult queryResult);

    protected static native String gorgonzolaQueryResultToString(QueryResult queryResult);

    protected static native void gorgonzolaQueryResultResetIterator(QueryResult queryResult);

    // FlatTuple
    protected static native void gorgonzolaFlatTupleDestroy(FlatTuple flatTuple);

    protected static native Value gorgonzolaFlatTupleGetValue(FlatTuple flatTuple, long index);

    protected static native String gorgonzolaFlatTupleToString(FlatTuple flatTuple);

    // DataType
    protected static native long gorgonzolaDataTypeCreate(
            DataTypeID id, DataType childType, long numElementsInArray);

    protected static native DataType gorgonzolaDataTypeClone(DataType dataType);

    protected static native void gorgonzolaDataTypeDestroy(DataType dataType);

    protected static native boolean gorgonzolaDataTypeEquals(DataType dataType1, DataType dataType2);

    protected static native DataTypeID gorgonzolaDataTypeGetId(DataType dataType);

    protected static native DataType gorgonzolaDataTypeGetChildType(DataType dataType);

    protected static native long gorgonzolaDataTypeGetNumElementsInArray(DataType dataType);

    // Value
    protected static native Value gorgonzolaValueCreateNull();

    protected static native Value gorgonzolaValueCreateNullWithDataType(DataType dataType);

    protected static native boolean gorgonzolaValueIsNull(Value value);

    protected static native void gorgonzolaValueSetNull(Value value, boolean isNull);

    protected static native Value gorgonzolaValueCreateDefault(DataType dataType);

    protected static native <T> long gorgonzolaValueCreateValue(T val);

    protected static native Value gorgonzolaValueClone(Value value);

    protected static native void gorgonzolaValueCopy(Value value, Value other);

    protected static native void gorgonzolaValueDestroy(Value value);

    protected static native Value gorgonzolaCreateMap(Value[] keys, Value[] values);

    protected static native Value gorgonzolaCreateList(Value[] values);

    protected static native Value gorgonzolaCreateList(DataType type, long numElements);

    protected static native long gorgonzolaValueGetListSize(Value value);

    protected static native Value gorgonzolaValueGetListElement(Value value, long index);

    protected static native DataType gorgonzolaValueGetDataType(Value value);

    protected static native <T> T gorgonzolaValueGetValue(Value value);

    protected static native String gorgonzolaValueToString(Value value);

    protected static native InternalID gorgonzolaNodeValGetId(Value nodeVal);

    protected static native String gorgonzolaNodeValGetLabelName(Value nodeVal);

    protected static native long gorgonzolaNodeValGetPropertySize(Value nodeVal);

    protected static native String gorgonzolaNodeValGetPropertyNameAt(Value nodeVal, long index);

    protected static native Value gorgonzolaNodeValGetPropertyValueAt(Value nodeVal, long index);

    protected static native String gorgonzolaNodeValToString(Value nodeVal);

    protected static native InternalID gorgonzolaRelValGetId(Value relVal);

    protected static native InternalID gorgonzolaRelValGetSrcId(Value relVal);

    protected static native InternalID gorgonzolaRelValGetDstId(Value relVal);

    protected static native String gorgonzolaRelValGetLabelName(Value relVal);

    protected static native long gorgonzolaRelValGetPropertySize(Value relVal);

    protected static native String gorgonzolaRelValGetPropertyNameAt(Value relVal, long index);

    protected static native Value gorgonzolaRelValGetPropertyValueAt(Value relVal, long index);

    protected static native String gorgonzolaRelValToString(Value relVal);

    protected static native Value gorgonzolaCreateStruct(String[] fieldNames, Value[] fieldValues);

    protected static native String gorgonzolaValueGetStructFieldName(Value structVal, long index);

    protected static native long gorgonzolaValueGetStructIndex(Value structVal, String fieldName);

    protected static native String gorgonzolaGetVersion();

    protected static native long gorgonzolaGetStorageVersion();
}
