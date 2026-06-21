import gorgonzola from "./index.js";

// Re-export everything from the CommonJS module
export const Database = gorgonzola.Database;
export const Connection = gorgonzola.Connection;
export const PreparedStatement = gorgonzola.PreparedStatement;
export const QueryResult = gorgonzola.QueryResult;
export const VERSION = gorgonzola.VERSION;
export const STORAGE_VERSION = gorgonzola.STORAGE_VERSION;
export default gorgonzola;
