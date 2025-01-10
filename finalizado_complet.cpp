#include <iostream>
#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <regex>
using namespace std;

void limpieza() {
    system("clear");
}

bool esDireccionIP(string direccion_IP) {
    regex patron("^\\b\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");

    if (regex_match(direccion_IP, patron))
        return true;
    else
        return false;
}
void nmap() {
    int existencia = system("which nmap > /dev/null");
    int opcion;
    string direccion_IP;
    string commmand;

    if (existencia == 0) {
        cout << "Bienvenido al menu de opciones de nmap" << endl;
        cout << "\t[1] Escaneo Basico" << endl;
        cout << "\t[2] Escaneo Rapido" << endl;
        cout << "\t[3] Escaneo completo" << endl;
        cout << "\t[4] Escaneo con Busqueda de vulnerabilidades (ruidoso)" << endl;
        cout << "\t[5] Escaneo Super Rapido (ruidoso)" << endl;
        cout << "\n[+] Ingresa el numero de opcion que deseas: ";
        cin >> opcion;
        cout << "[+] Ingresa la direccion IP: ";
        cin >> direccion_IP;

        bool verificacion = esDireccionIP(direccion_IP);
        if (verificacion == true)
            switch (opcion) {
            case 1:
                system(("nmap " + direccion_IP).c_str());

                break;
            case 2:
                system(("nmap -sS -T3 " + direccion_IP).c_str());

                break;
            case 3:
                system(("nmap -sS --open -p- --min-rate 5000 " + direccion_IP).c_str());

                break;
            case 4:
                system(("nmap -sC --script 'vuln and safe' --min-rate 5000" + direccion_IP).c_str());
                
                break;
            case 5:
                system(("nmap --min-rate 10000 -sS -T4 " + direccion_IP).c_str());

                break;
            default:
                cout << "Opcion brindada no es correcta";
                return;
                break;
            }

        system(commmand.c_str());

    }
    else if (existencia == 1) {
        cout << "El script no existe asi que vamos a instalarlo" << endl;
        cout << "Recordar que para instalar debemos pedirte tu contraseña" << endl;
        system("sudo apt-get install nmap");

    }
    else {
        cout << "Error al verificar la existencia del script" << endl;
        return;
    }
}

void dashboard() {
    limpieza();
    cout << " --------------" << endl;
    cout << "// DASHBOARD //" << endl;
    cout << "--------------" << endl;
}

void homework(sql::Connection *con) {
    limpieza();
    cout << " -------------" << endl;
    cout << "// HOMEWORK //" << endl;
    cout << "-------------" << endl;

    try {
        sql::Statement *stmt;
        sql::ResultSet *res;
        std::string querytly;

        stmt = con->createStatement();
        stmt->execute("USE Homework");
        system("clear");
        cout << "\n" << endl;

        while (true) {
            cout << "\n\n\t01 -- VIEW TASK" << endl;
            cout << "\t02 -- DELETE TASK" << endl;
            cout << "\t03 -- CREATE TASK\n\n" << endl;
            cout << "\e[1;91mQUERY>>\e[0m ";
            getline(cin, querytly);

            if (querytly == "\\exit") {
                cout << "\e[0m" << endl;
                break;
            }
            if (querytly == "\\clear") {
                limpieza();
            }
            std::system("clear");
            try {
                if (querytly == "01") {
                    //std::string fullQuery = "SELECT * FROM tasks";
                    std::string fullQuery = "SELECT TASK_TITTLE, TASK_DESCRIPT FROM tasks";
                    res = stmt->executeQuery(fullQuery);
                    sql::ResultSetMetaData *metaData = res->getMetaData();
                    int columnCount = metaData->getColumnCount();
                    cout << std::endl;
					//ENCABEZADO DE LOS DATOS DE LAS TABLAS
                    for (int i = 1; i <= columnCount; i++) {
                        cout << std::left << std::setw(15) << metaData->getColumnName(i);                        
                    }

                    cout << std::endl;
                    while (res->next()) {
                        for (int i = 1; i <= columnCount; i++) {
                            cout << std::left << std::setw(15) << res->getString(i);
                           	//cout<<std::endl;
                        }
                        cout << std::endl;
                    }
                    cout << std::endl;
                    delete res;
                } else if (querytly == "02") {
                    std::system("clear");
                    std::string fullQuery = "SELECT IDv4, TASK_DESCRIPT FROM tasks";
                    res = stmt->executeQuery(fullQuery);
                    cout << "\nTask ID\tTask Description" << endl;
                    while (res->next()) {
                        cout << res->getInt(1) << "\t" << res->getString(2) << endl;
                    }
                    int taskID;
                    cout << "\nEnter the Task ID to delete: ";
                    cin >> taskID;
                    std::string deleteQuery = "DELETE FROM tasks WHERE IDv4 = ?";
                    sql::PreparedStatement *prepStmt = con->prepareStatement(deleteQuery);
                    prepStmt->setInt(1, taskID);
                    prepStmt->executeUpdate();
                    delete prepStmt;
                    cout << "Task with ID " << taskID << " deleted successfully." << endl;
                } else if (querytly == "03") {
                    string taskTitle, taskDescription;
                    //cin.ignore();
                    cout << "Enter the task TITLE: ";
                    getline(cin, taskTitle);
                    cout << "Enter the task DESCRIPTION: ";
                    getline(cin, taskDescription);
                    string insertQuery = "INSERT INTO tasks (TASK_TITTLE, TASK_DESCRIPT) VALUES (?, ?)";
                    sql::PreparedStatement *prepStmt = con->prepareStatement(insertQuery);
                    prepStmt->setString(1, taskTitle);
                    prepStmt->setString(2, taskDescription);
                    prepStmt->executeUpdate();
                    delete prepStmt;
                    cout << "Task added successfully." << endl;
                } else {
                    if (querytly == "\\exit" || querytly == "\\clear")
                        cout << "" << endl;
                    else
                        if (querytly == "0")
                            cout << "Valor incorrecto. Por favor, introduce '01' o '02'." << endl;
                        else
                            cout << "" << endl;
                }
            } catch (sql::SQLException &e) {
                cout << "Error MYSQL: " << e.what() << endl;
            }
        }

        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "Error MYSQL: " << e.what() << endl;
    }
}

void databases() {
    limpieza();
    cout << " ----------" << endl;
    cout << "// MYSQL //" << endl;
    cout << "----------" << endl;
    try {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        std::string querytly;
        std::string database;
        std::string name;
        std::string password;

        driver = sql::mysql::get_mysql_driver_instance();
        std::cout << "Ingresa el nombre de la DB: ";
        cin.ignore();
        std::getline(std::cin, database);
        std::cout << "Ingresa el usuario de la DB: ";
        std::getline(std::cin, name);
        std::cout << "Ingresa la contraseña de la DB: ";
        std::getline(std::cin, password);
        con = driver->connect("tcp://107.161.179.89:3306", name, password);
        stmt = con->createStatement();
        stmt->execute("USE " + database);
        system("clear");
        cout << "\n" << endl;
        
        while (true) {
            cout << "\e[1;91mQUERY>> ";
            std::getline(std::cin, querytly);

            if (querytly.empty()) {
                continue;
            }
            if (querytly == "EXIT") {
                cout << "\e[0m" << endl;
                break;
            }
            if (querytly == "CLEAR") {
                limpieza();
                continue;
            }
            try {
                stmt = con->createStatement();
                res = stmt->executeQuery(querytly);

                // Obtener metadata de las columnas
                sql::ResultSetMetaData *metaData = res->getMetaData();
                int columnCount = metaData->getColumnCount();

                // Anchos de columna ajustados
                int maxColumnWidth = 18; // Ancho máximo de las columnas

                // Encabezado de la tabla
                for (int i = 1; i <= columnCount; i++) {
                    string columnName = metaData->getColumnName(i);
                    if (columnName.length() > maxColumnWidth) {
                        columnName = columnName.substr(0, maxColumnWidth - 3) + "..."; // Truncar si es demasiado largo
                    }
                    cout << "+-" << std::setw(maxColumnWidth) << std::setfill('-') << "-" << std::setfill(' ');
                }
                cout << "+" << endl;

                // Nombres de columnas
                for (int i = 1; i <= columnCount; i++) {
                    string columnName = metaData->getColumnName(i);
                    if (columnName.length() > maxColumnWidth) {
                        columnName = columnName.substr(0, maxColumnWidth - 3) + "..."; // Truncar si es demasiado largo
                    }
                    cout << "| " << std::setw(maxColumnWidth) << std::left << columnName;
                }
                cout << "|" << endl;

                // Línea separadora
                for (int i = 1; i <= columnCount; i++) {
                    cout << "+-" << std::setw(maxColumnWidth) << std::setfill('-') << "-" << std::setfill(' ');
                }
                cout << "+" << endl;

                // Filas de resultados
                while (res->next()) {
                    for (int i = 1; i <= columnCount; i++) {
                        string data = res->getString(i);
                        if (data.length() > maxColumnWidth) {
                            data = data.substr(0, maxColumnWidth - 3) + "..."; // Truncar si es demasiado largo
                        }
                        cout << "| " << std::setw(maxColumnWidth) << std::left << data;
                    }
                    cout << "|" << endl;
                }

                // Línea final
                for (int i = 1; i <= columnCount; i++) {
                    cout << "+-" << std::setw(maxColumnWidth) << std::setfill('-') << "-" << std::setfill(' ');
                }
                cout << "+" << endl;

                delete res;
            } catch (sql::SQLException &e) {
                cout << "Error MYSQL: " << e.what() << endl;
            }
        }

        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "Error MYSQL: " << e.what() << endl;
    }
}

void showip() {
    system("clear");
    std::system("ip a");
    std::cout<<endl;
    std::cout<<"\n";
}

void servidorPython() {
    std::system("clear");
    int port;
    std::cout << "Ingresa el puerto por donde se usara python" << std::endl;
    std::cout << ">> ";
    std::cin >> port;
    std::string portStr = std::to_string(port);
    std::string comando = "python -m http.server " + portStr;
    std::system(comando.c_str());
}

/*void upload() {
	system("clear");
	const int total = 50;
	const char bar_char = '=';
	cout<<"starting service Apache2"<<endl;
	for (int i=0; i<=total;i++) {
		cout<<"[";
		for(int j=0; j<i; j++) {
			cout << bar_char;
		}
		for(int j=i; j<total;j++) {
			cout<<" ";
		}
		cout<<"] "<<int(i*100/total)<<"%\r";
		cout.flush();
		usleep(100000);
	}
	cout<<endl;
	cout<<"¡Proceso completado!"<<endl;

}*/
//
// 		SEGUNDA OPCION DE UPLOAD
//

void upload() {
    system("clear");
    

    // Datos para la carga en la máquina de THM
    string thm_username;
    string thm_ip;
    string local_file_path;
    string remote_directory;

	cout << "Enter the username of the machine: ";
	cin >> thm_username;
	cout << "Enter the ip of the machine: ";
	cin >> thm_ip;
	cout << "Enter the path of the local file: ";
	cin >> local_file_path;
	cout << "Enter the remote directory path the machin: ";
	cin >> remote_directory;
    cout << "Cargando archivo a TryHackMe..." << endl;

    // Construir el comando scp
    string scp_command = "scp " + local_file_path + " " + thm_username + "@" + thm_ip + ":" + remote_directory;
    // Ejecutar el comando scp
    system(scp_command.c_str());
    cout << "Archivo cargado exitosamente." << endl;
}

void download() {
    string ip, port, filename;
    char typeHttp;
    limpieza();
    cout << "enter the ip: ";
    cin >> ip;
    cout << "enter the port: ";
    cin >> port;
    cout << "enter the file name: ";
    cin >> filename;
    //cout << "https[Y] or http[N] ?\n[Y/N]" << endl;
    cout << "Is the file on the local network ?\n[Y/N]";
    cin >> typeHttp;

    if (typeHttp == 'Y' || typeHttp == 'y') {
        system(("wget https://" + ip + ":" + port + "/" + filename).c_str());
    } else if (typeHttp == 'N' || typeHttp == 'n') {
        system(("wget http://" + ip + ":" + port + "/" + filename).c_str());
    } else {
        cout << "Invalid input. Please enter Y or N." << endl;
    }
}
void help() {
    limpieza();
    cout << " ---------" << endl;
    cout << "// HELP //" << endl;
    cout << "--------" << endl;
    cout << " \\help     >>   HELP" << endl;
    cout << " \\dash     >>   EXPEDIENT" << endl;
    cout << " \\mysql    >>   MySQL" << endl;
    cout << " \\exit     >>   EXIT" << endl;
    cout << " \\task     >>   HOMEWORK - TASK'S" << endl;
    cout << " \\ip       >>   SHOW IP" << endl;
    cout << " \\server   >>   SERVIDOR PYTHON" << endl;
    cout << " \\nmap     >>   NMAP - PORT SCANNING" << endl;
    cout << "\n\n\n";
}

int main() {
    string command;
    limpieza();
    try {
        //sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        //sql::Connection *con = driver->connect("tcp://127.0.0.1:3306", "Work", "deadgun");

        do {
            cout << "[0]>> ";
            cin >> command;

            
            if (command == "\\dash") {
                dashboard();
            } else if (command == "\\mysql") {
                databases();
            } else if (command == "\\help") {
                help();
            } else if (command == "\\task") {
                //homework(con);
            } else if (command == "\\ip") {
                showip();
            } else if (command == "\\server") {
                servidorPython();
            } else if (command == "\\clear") {
                limpieza();
            } else if (command == "clear") {
                limpieza();
            } else if (command == "\\exit") {
                break;
            } else if (command == "\\nmap") {
            	nmap();
            } else if (command == "\\upload") {
            	upload();
            } else if (command == "\\download") {
            	download();
            } else {
                cout << "Comando no reconocido. Inténtalo de nuevo." << endl;
            }
        } while (true);

        //delete con;
    } catch (sql::SQLException &e) {
        //cout << "Error MYSQL: " << e.what() << endl;
        //cout<<"No esta activado el servicio de MYSQL"<<endl;
        cout<<"\e[1;91m";
        //cout<<"MySQL ISN'T ACTIVE"<<endl;
		cout<<" ========================================================================="<<endl;
		system("figlet TURN ON MYSQL");
		cout<<" ========================================================================="<<endl;

        cout<<"\e[0m";
        //system("service mysql start");
    }

    return 0;
}
