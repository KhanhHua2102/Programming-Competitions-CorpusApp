// src/contexts/ExportProblemsContext.js
import React, { createContext, useContext, useState } from 'react';

const ExportProblemsContext = createContext();

export function ExportProblemsProvider({ children }) {
  const [exportProblems, setExportProblems] = useState([]);

  return (
    <ExportProblemsContext.Provider value={[exportProblems, setExportProblems]}>
      {children}
    </ExportProblemsContext.Provider>
  );
}

export function useExportProblems() {
  const context = useContext(ExportProblemsContext);
  if (!context) {
    throw new Error(
      'useExportProblems must be used within an ExportProblemsProvider'
    );
  }
  return context;
}
