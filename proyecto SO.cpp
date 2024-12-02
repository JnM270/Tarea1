#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>


struct Proceso {
    std::string actividad; // Nombre de la actividad
    int ti; // Tiempo inicial
    int t;  // Tiempo de la actividad
    int tf; // Tiempo final
    int T;  // Tiempo total
    int E;  // Tiempo de espera
    double I; // Índice de servicio
    int id; // Permite que los procesos se muestren en el orden original del archivo
};

// Función para leer los datos del archivo de texto
void leerProcesos(const std::string &archivo, std::vector<Proceso> &procesos) {
    std::ifstream file(archivo.c_str());
    if (!file) {
        std::cerr << "No se pudo abrir el archivo: " << archivo << std::endl; 
        exit(1);
    }
    
    std::string lin;
    int id = 0;
    while (std::getline(file, lin)) {
        std::istringstream iss(lin);
        std::string actividad;
        int ti, t;
        if (iss >> actividad >> ti >> t) {
            Proceso proceso = {actividad, ti, t, 0, 0, 0, 0.0, id++}; // La variable proceso se utiliza para crear y manejar un solo proceso
            procesos.push_back(proceso);
        }
    }
    file.close();
}

// Algoritmo FIFO
void calcFIFO(std::vector<Proceso> &procesos, std::vector<int> &clock_val) {
    int clock = 0; // Reloj del sistema
    std::vector<bool> proc_ejecutado(procesos.size(), false); 

    for (size_t proc_ejecutados = 0; proc_ejecutados < procesos.size();) {
        bool procCompletado = false;

        // Buscar proceso disponible para ejecutar (Orden FIFO)
        for (size_t i = 0; i < procesos.size(); ++i) {
            if (!proc_ejecutado[i] && procesos[i].ti <= clock) {
                
                // Operaciones
                procesos[i].tf = clock + procesos[i].t;       
                procesos[i].T = procesos[i].tf - procesos[i].ti; 
                procesos[i].E = procesos[i].T - procesos[i].t;   
                procesos[i].I = static_cast<double>(procesos[i].t) / procesos[i].T;

                clock_val.push_back(clock); // Guarda el valor del reloj
                clock = procesos[i].tf;      
                proc_ejecutado[i] = true;    // El proceso se confirma como completado
                proc_ejecutados++;           // Aumenta el contador de procesos completados
                procCompletado = true;
                break;
            }
        }

        // En caso de no encontrar ningún proceso para ejecutar (NO LOOP), se incrementa el reloj
        if (!procCompletado) {
            clock++;
        }
    }
}

// Algoritmo LIFO
void calcLIFO(std::vector<Proceso> &procesos, std::vector<int> &clock_val) {
    int clock = 0; // Reloj del sistema
    std::vector<bool> proc_ejecutado(procesos.size(), false); // Vector para marcar procesos ejecutados

    for (size_t proc_ejecutados = 0; proc_ejecutados < procesos.size();) {
        bool procCompletado = false;

        // Buscar proceso disponible para ejecutar (Orden LIFO)
        for (int i = procesos.size() - 1; i >= 0; --i) {
            if (!proc_ejecutado[i] && procesos[i].ti <= clock) {
                
                // Operaciones
                procesos[i].tf = clock + procesos[i].t;       
                procesos[i].T = procesos[i].tf - procesos[i].ti; 
                procesos[i].E = procesos[i].T - procesos[i].t;   
                procesos[i].I = static_cast<double>(procesos[i].t) / procesos[i].T; 

                clock_val.push_back(clock); // Guarda el valor del reloj
                clock = procesos[i].tf;      
                proc_ejecutado[i] = true;    // El proceso se confirma como completado
                proc_ejecutados++;           // Aumenta el contador de procesos completados 
                procCompletado = true;
                break;
            }
        }

        // En caso de no encontrar ningún proceso para ejecutar (NO LOOP), se incrementa el reloj
        if (!procCompletado) {
            clock++;
        }
    }
}

// Función para imprimir resultados
void Resultados(const std::vector<Proceso> &procesos, const std::vector<int> &clock_val, const std::string &algoritmo) {
    std::cout << "\nTabla " << algoritmo << ":\n";
    std::cout << std::setw(10) << "Actividad" << std::setw(4) << "ti" << std::setw(6) << "t" 
              << std::setw(6) << "tf" << std::setw(6) << "T" << std::setw(6) << "E" 
              << std::setw(10) << "I" << std::setw(10) << "Clock" << std::endl;

    double totalT = 0.0, totalE = 0.0, totalI = 0.0;
    for (size_t i = 0; i < procesos.size(); ++i) {
        std::cout << std::setw(10) << procesos[i].actividad 
                  << std::setw(4) << procesos[i].ti 
                  << std::setw(6) << procesos[i].t 
                  << std::setw(6) << procesos[i].tf
                  << std::setw(6) << procesos[i].T 
                  << std::setw(6) << procesos[i].E 
                  << std::setw(10) << std::fixed << std::setprecision(6) << procesos[i].I
                  << std::setw(10) << clock_val[i] << std::endl;
        
        totalT += procesos[i].T;
        totalE += procesos[i].E;
        totalI += procesos[i].I; 
    }
    
    double promedioT = totalT / procesos.size();
    double promedioE = totalE / procesos.size();
    double promedioI = totalI / procesos.size(); 
    
    std::cout << "\nPromedio T: " << promedioT 
              << "\nPromedio E: " << promedioE 
              << "\nPromedio I: " << promedioI << std::endl;
}

int main() {
    std::vector<Proceso> procesos; // Es un vector que almacena variables proceso
    std::string archivo = "procesos.txt";

    leerProcesos(archivo, procesos);

    // FIFO
    std::vector<int> clockVal_fifo;
    std::vector<Proceso> proc_fifo = procesos;
    calcFIFO(proc_fifo, clockVal_fifo);
    Resultados(proc_fifo, clockVal_fifo, "FIFO");

    // LIFO
    std::vector<int> clockVal_lifo;
    std::vector<Proceso> proc_lifo = procesos;
    calcLIFO(proc_lifo, clockVal_lifo);
    Resultados(proc_lifo, clockVal_lifo, "LIFO");

    return 0;
}

