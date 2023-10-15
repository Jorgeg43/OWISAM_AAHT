//JORGE GÓMEZ MARTÍN ---> jogome13@ucm.es
// Universidad Europea de Madrid,2023
// TFM Máster en Seguridad de las TIC.
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <string>
#include <limits>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

const int maxColumnas = 100;
const int maxFilas = 100;
string interfazCaptura = "wlan1mon";
void ficheroSSIDGenericos(string carpetaProyecto){
    string comando = "cd " + carpetaProyecto + " && git clone https://gist.github.com/da795e571fb5f91f9e86a27f2c2f626f.git";
    system(comando.c_str());
}
int contarLineas(const std::string &nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return 0;
    }

    int contador = 0;
    std::string linea;
    while (std::getline(archivo, linea)) {
        contador++;
    }

    archivo.close();
    return contador;
}
void PantallaInicio(){
    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << endl << "\033[31mJorge Gomez Martin\033[0m jogome13@ucm.es" << endl;
    cout << "\033[31m2023, Universidad Europea de Madrid\033[0m TFM Master en Seguridad de las TIC" << endl;
    cout << endl;
    cout << endl;
    cout << "Presentada por los hermanos Tarrasco en 2013, OWISAM es una metodología abierta para la auditoria de redes WIFI." << endl;
    cout << "Mas informacion en: https://www.owisam.org/index.php?title=P%C3%A1gina_principal" << endl << endl;
    cout << "Esta herramienta se presenta como una ayuda para el auditor que automatiza alguno de los controles más relevantes generando un informe preliminar de manera automatica." << endl << endl;
    cout << "[Presiona enter para continuar]" << endl;
    cin.get();
    system("clear");

}

bool ComprobarPrivilegios() {
    // Si se está corriendo con privilegios devuelve 1.
    if (geteuid() == 0) {
        return 1;
    } else {
        return 0;
    }
}

bool ConfiguracionDeRed(){
    system("clear");
    int input;
    string interfaz,comando;
    cout << "[CONFIGURACIÓN DE RED]" << endl;
    cout << "[1] Seleccionar interfaz de captura" << endl;
    cout << "[2] Atrás" << endl;
    // Validar entrada usuario
    while (!(cin >> input)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Entrada inválida. Introduzca un número: ";
    }
    switch(input){
        case 1:
            cout << "Interfaces de red activas:" << endl;
			cout << endl << "Lanzando comando: " << comando << endl;
            system("ip addr show");
            cout << "¿En qué interfaz deseas habilitar la captura de paquetes? [introduzca su nombre]:";
            cin >> interfaz;
            interfazCaptura = interfaz + "mon";
            comando = "sudo airmon-ng start " + interfaz;
            cout << comando << endl;
            system(comando.c_str());
            return 1;
        break;
        case 2:
            return 1;

        default:
            cout << "Comando no válido" << endl;
            return 0;
        break;



    }


}

bool GenerarInforme(string carpetaProyecto){

    string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    ifstream datosNP1(rutaDatosNP1);
    for (int i=0; i<6 ; i++){
        getline(datosNP1,lineaLeida);
    }
    getline(datosNP1,rutaProyecto);
    getline(datosNP1,carpetaProyecto);
    getline(datosNP1,nombreProyecto);
    getline(datosNP1,nombreAuditor);
    getline(datosNP1,empresaAuditora);
    getline(datosNP1,empresaAuditada);
    // Crear fichero html para el informe
    string comando, rutaInforme;
    rutaInforme = carpetaProyecto + "/informeAuditoria.htm";
    comando = "touch " + rutaInforme;
    ofstream informeAuditoria(rutaInforme);
    // Cabeceras y portada
    informeAuditoria << R"(<!DOCTYPE html>
                                <html lang="es">
                                <head>
                                    <meta charset="UTF-8">
                                    <meta name="viewport" content="width=device-width, initial-scale=1.0">
                                    <title>Informe de Auditoría</title>
                                    <style>
                                        body {
                                            font-family: Arial, sans-serif;
                                            margin: 20px;
                                        }

                                        h1 {
                                            text-align: center;
                                            font-size: 24px;
                                        }

                                        h2 {
                                            font-size: 20px;
                                            margin-top: 20px;
                                        }

                                        h3 {
                                            font-size: 18px;
                                            margin-top: 18px;
                                        }

                                        p {
                                            margin: 10px 0;
                                        }

                                        ul {
                                            list-style-type: decimal;
                                            margin-left: 20px;
                                        }ControlWifite

                                         ul1 {
                                            list-style-type: disc;
                                        }

                                        table {
                                            width: 100%;
                                            border-collapse: collapse;
                                            margin-top: 20px;
                                        }

                                        th, td {
                                            border: 1px solid #ccc;
                                            padding: 8px;
                                            text-align: left;
                                        }
                                        .rojo {
                                            background-color: red;
                                        }
                                        .verde {
                                            background-color: green;
                                        }
                                    </style>
                                </head>
                                <body>
                                    <h1>Informe de Auditoría</h1>)";
    informeAuditoria << "<h2>" << nombreProyecto << "</h2>";
    // Incluir fecha
    // Obtener la fecha actual del sistema
    time_t tiempo_actual = time(nullptr);
    tm* fecha = localtime(&tiempo_actual);
    // Nombres de los meses en un array
    const char* meses[] = {
        "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"
    };
    // Formatear la fecha en el string
    string fecha_formateada = to_string(fecha->tm_mday) + " de " + meses[fecha->tm_mon] + " de " + to_string(fecha->tm_year + 1900);

    informeAuditoria << "<p>Fecha de auditoría: " << fecha_formateada << "</p>";
    informeAuditoria << "<p>Empresa auditora: " << empresaAuditora << "</p>";
    informeAuditoria << "<p>Empresa auditada: " << empresaAuditada << "</p>";
    informeAuditoria << "<p>Auditor: " << nombreAuditor << "</p>";
    bool fp004 = 0, di002 = 0, au004 = 0, cp004 = 0, cf001, if003 = 0, ds001 = 0,ct001 = 0;
    bool vulnFp004 = 0, vulnDi002 = 0, vulnAu004 = 0, vulnCp004 = 0, vulnCf001 = 0, vulnIf003 = 0, vulnDs001 = 0,vulnCt001 = 0;
    getline(datosNP1,lineaLeida);
    getline(datosNP1,lineaLeida);
    if (lineaLeida == "1")
        fp004 = 1;
    getline(datosNP1,lineaLeida);
    getline(datosNP1,lineaLeida);
    if (lineaLeida == "1")
        di002 = 1;
    getline(datosNP1,lineaLeida);
    getline(datosNP1,lineaLeida);
    if (lineaLeida == "1")
        au004 = 1;
    getline(datosNP1,lineaLeida);
    getline(datosNP1,lineaLeida);
    if (lineaLeida == "1")
        cp004 = 1;
    getline(datosNP1,lineaLeida);
    getline(datosNP1,lineaLeida);
    if (lineaLeida == "1")
        cf001 = 1;
    getline(datosNP1,lineaLeida);
    getline(datosNP1,lineaLeida);
    if (lineaLeida == "1")
        if003 = 1;
    getline(datosNP1,lineaLeida);
    getline(datosNP1,lineaLeida);
    if (lineaLeida == "1")
        ds001 = 1;
	getline(datosNP1,lineaLeida);
    getline(datosNP1,lineaLeida);
    if (lineaLeida == "1")
        ct001 = 1;
    informeAuditoria << "<h2>Controles Auditados</h2>";
    // Comprobar controles auditados y añadirlos al índice
    informeAuditoria << "<ul>" << endl;
    if (fp004){
    informeAuditoria << R"(
        <li><a href="#fp004">OWISAM-FP-004</a></li>)";
    }
    if (di002){
    informeAuditoria << R"(
        <li><a href="#di002">OWISAM-DI-002</a></li>)";
    }
     if (au004){
    informeAuditoria << R"(
        <li><a href="#au004">OWISAM-AU-004</a></li>)";
    }

    if (cp004){
    informeAuditoria << R"(
        <li><a href="#cp004">OWISAM-CP-004</a></li>)";
    }

    if (cf001){
    informeAuditoria << R"(
        <li><a href="#cf001">OWISAM-CF-001</a></li>)";
    }

    if (if003){
    informeAuditoria << R"(
        <li><a href="#if003">OWISAM-IF-003</a></li>)";
    }

    if (ds001){
    informeAuditoria << R"(
        <li><a href="#ds001">OWISAM-DS-001</a></li>)";
    }
    informeAuditoria << "</ul>" << endl;
    informeAuditoria << "<ul>";
    informeAuditoria << R"(
        <li><a href="#resumen">Resumen ejecutivo</a></li>)";
    informeAuditoria << "</ul>" << endl;
    // Añadir reporte de los controles auditados
    //Reporte OWISAM-FP-004
    if (fp004){
        string rutaFp004 = carpetaProyecto + "/OWISAM-FP-004/OWISAM-FP-004.jgm";
        informeAuditoria << R"(<h2 id="fp004">OWISAM-FP-004</h2>
        <h3> Descripción del control </h3>
        <p>En el contexto empresarial actual, la conectividad inalámbrica es esencial para la eficiencia y la productividad. Sin embargo, la proliferación de dispositivos personales y la facilidad para configurar redes WiFi personales pueden dar lugar a una brecha de seguridad potencialmente grave. La presencia de redes WiFi no autorizadas dentro de una empresa puede exponer datos confidenciales y comprometer la integridad de la red corporativa.</p>
        <p>El control OWISAM-FP-004 busca puntos de acceso no autorizados dentro del alcance de sus dispositivos corporativos.</p>)";
        ifstream file(rutaFp004);
        string line;
        string matriz[maxFilas][maxColumnas];
        int row = 0;
        int cols = 0;
        while (getline(file, line)) {
            string token;
            istringstream tokenStream(line);
            int col = 0;

            while (getline(tokenStream, token, ',')) {
                matriz[row][col] = token;
                col++;
            }

            cols = std::max(cols, col);
            row++;
        }
        file.close();
        informeAuditoria << "<h3>Resultados del control</h3>";
        if (row == 1){
            informeAuditoria << "<p> No se han encontrado redes ajenas a su organización dentro de su entorno inalámbrico.</p>";
            informeAuditoria << "<h3>Recomendaciones</h3>";
            informeAuditoria << "<p>Pese a que no se han detectado redes ajenas en su entorno, recomendamos advertir a los usuarios del riesgo de emplear ajenas, especialmente aquellas con privacidad abierta y que no empleen métodos de cifrado para el intercambio de datos.</p>";
        }
        else{
            vulnFp004 = 1;
            informeAuditoria << "<p>Se han encontrado " << row - 1 << " redes ajenas a su organización dentro de su entorno inalámbrico, que exponemos a continuación:</p>";
            informeAuditoria << R"(<table>
            <tr>
                <th>BSSID</th>
                <th>ESSID</th>
                <th>Vista por primera vez</th>
                <th>Canal</th>
                <th>Velocidad</th>
                <th>Privacidad</th>
                <th>Cifrado</th>
                <th>Autenticación</th>
                <th>Potencia</th>
                <th>Balizas</th>
            </tr>)";

        for (int i = 1; i < row; i++) {
            informeAuditoria << "<tr>";
            for (int j = 0; j < cols; j++) {
                informeAuditoria << "<td>";
                informeAuditoria << matriz[i][j];
                informeAuditoria << "</td>";
            }
            informeAuditoria << "</tr>";
        }
        informeAuditoria << "</table>";
        informeAuditoria << "<h3>Recomendaciones</h3>";
        informeAuditoria << "<ul1>";
        informeAuditoria << "<li>Recomendamos identificar si las direcciones físicas corresponden a dispositivos inalámbricos corporativos o algún dispositivo personal de un empleado.</li>";
        informeAuditoria << "<li>En caso de no determinar el origen del punto de acceso seria aconsejable advertir a los usuarios de dichas redes no corporativas para evitar su uso, especialmente de aquellas que tengan privacidad abierta.</li>";
        informeAuditoria << "<li>Advertir a los usuarios del riesgo de emplear ajenas, especialmente aquellas con privacidad abierta y que no empleen métodos de cifrado para el intercambio de datos.</li>";
        informeAuditoria << "</ul1>";
        }

    }
    //Reporte OWISAM-DI-002
    if (di002){
        string rutaDi002 = carpetaProyecto + "/OWISAM-DI-002/OWISAM-DI-002.jgm";
        ifstream file(rutaDi002);
        string line;
        string matriz[maxFilas][maxColumnas];
        int row = 0;
        int cols = 0;
        while (getline(file, line)) {
            string token;
            istringstream tokenStream(line);
            int col = 0;

            while (getline(tokenStream, token, ',')) {
                matriz[row][col] = token;
                col++;
            }

            cols = std::max(cols, col);
            row++;
        }
        file.close();
        informeAuditoria << R"(<h2 id="di002">OWISAM-DI-002</h2>
        <h3>Descripción del control </h3>
        <p>Este control busca posibles redes WIFI ocultas no autorizadas en el entorno inalámbrico de su organización</p>)";
        if (matriz[1][0] == "NO HAY REDES OCULTAS")
            informeAuditoria << "<h3>Resultados</h3><p>No se han encontrado redes ocultas en su entorno inalámbrico</p>" << endl;
        else{
            vulnDi002 = 1;
            informeAuditoria << "<h3>Resultados</h3><p>Se han encontrado las siguientes redes ocultas en su entorno inalámbrico:</p>" << endl;
            informeAuditoria << "<table><tr><th>BSSID</th><th>Potencia</th><th>Vista por primera vez</th></tr>";
            for(int i=1; i<row;i++){
                informeAuditoria << "<tr>";
                informeAuditoria << "<td>" << matriz[i][0] << "</td>" << "<td>" << matriz[i][8] << "</td>" << "<td>" << matriz[i][2] << "</td>";
                informeAuditoria << "</tr>";
            }
            informeAuditoria << "</table>";
        }
    }
    //Reporte OWISAM-AU-004
    if (au004){
        string rutaAu004 = carpetaProyecto + "/OWISAM-AU-004/OWISAM-AU-004.jgm";
        ifstream file(rutaAu004);
        string line;
        string matriz[maxFilas][maxColumnas];
        int row = 0;
        int cols = 0;
        getline(file,line);
        while (getline(file, line)) {
            string token;
            istringstream tokenStream(line);
            int col = 0;

            while (getline(tokenStream, token, ',')) {
                matriz[row][col] = token;
                col++;
            }

            cols = std::max(cols, col);
            row++;
        }
        file.close();
        informeAuditoria << R"(<h2 id="au004">OWISAM-AU-004</h2><h3>Descripción del control</h3><p>Este control captura la transación de credenciales WPA y crackea los hashes capturados para intentandar detectar contraseñas débiles fácilmentente descifrables.</p>)";
        if(matriz[0][0] == "0"){
            informeAuditoria << "<h3>Resultados</h3><p>No se han capturado credenciales débiles empleadas en sus redes inalámbricas.</p>" << endl;
        }
        else{
            vulnAu004 = 1;
            informeAuditoria << "<h3>Resultados</h3>" << endl << "<p>Se han capturado y descifrado credenciales débiles empleadas en " << row << " redes inalámbricas corporativas, las cuales exponemos a continuación:</p>" << endl;
            informeAuditoria << "<table>" << endl;
            informeAuditoria << "<tr><th>ESSID</th><th>BSSID</th><th>Contraseña</th><th>Seguridad</th></tr>" << endl;
            for(int i=0; i<row; i++){
                informeAuditoria << "<tr>";
                for(int j=0; j<cols; j+=2){
                    informeAuditoria << "<td>" << (matriz[i][j] = matriz[i][j].substr(2, matriz[i][j].length() - 3)) << "</td>";
                }
                informeAuditoria << "</tr>" << endl;
            }
            informeAuditoria << "</table>" << endl;
            informeAuditoria << "<h3>Recomendaciones</h3>" << endl;
            informeAuditoria << "<ul1>" << endl;
            informeAuditoria << "<li>Utilizar una contraseña larga y compleja: Es fundamental que se elija una contraseña que sea larga y compleja. Debe incluir una combinación de letras mayúsculas, minúsculas, números y caracteres especiales para hacerla más resistente a los ataques.</li>" << endl;
            informeAuditoria << "<li>Evitar información personal: Se debe evitar el uso de información personal, como nombres, fechas de nacimiento o palabras comunes, en la contraseña. Los atacantes suelen intentar adivinar contraseñas utilizando información pública sobre el individuo.</li>" << endl;
            informeAuditoria << "<li>No utilizar palabras del diccionario: Es importante evitar el uso de palabras que se encuentren en el diccionario, ya que los ataques de fuerza bruta pueden probar todas las palabras posibles. En su lugar, se debe optar por una combinación de letras aleatorias.</li>" << endl;
            informeAuditoria << "<li>Cambiar las contraseñas periódicamente: Se recomienda cambiar la contraseña del Wi-Fi de forma periódica para aumentar la seguridad. Esto dificulta que los atacantes mantengan acceso a la red.</li>" << endl;
            informeAuditoria << "</ul1>" << endl;
        }
    }
    //Reporte OWISAM-CP-004
    if (cp004){
        string rutaCp004 = carpetaProyecto + "/OWISAM-CP-004/OWISAM-CP-004.jgm";
        informeAuditoria << R"(<h2 id="cp004">OWISAM-CP-004</h2>
        <h3> Descripción del control </h3>
        <p>El control OWISAM-CP-004 busca redes corporativas que empleen protocolos de cifrado de datos obsoletos o inseguros, tales como el protoclo WEP o el protocolo TKIP.</p>)" << endl;
        ifstream file(rutaCp004);
        string line;
        string matriz[maxFilas][maxColumnas];
        getline(file,line);
        int row = 0;
        int cols = 0;
        while (getline(file, line)) {
            string token;
            istringstream tokenStream(line);
            int col = 0;

            while (getline(tokenStream, token, ',')) {
                matriz[row][col] = token;
                col++;
            }

            cols = std::max(cols, col);
            row++;
        }
        file.close();
        bool cifradoInseguro[row][1];
        int detecciones = 0;
        for (int i=0;i<row;i++){
            if(matriz[i][6] == " WEP" || matriz[i][6] == " TKIP"){
                cifradoInseguro[i][0] = 1;
                detecciones += 1;
            }
            else
                cifradoInseguro[i][0] = 0;
        }
        informeAuditoria << "<h3>Resultados</h3>" << endl;

        if (detecciones == 0){
            informeAuditoria << "<p>El control no ha encontrado redes corporativas que empleen cifrados no seguros, tras escanear las siguientes redes corporativas: </p>" << endl;
            informeAuditoria << "<table>" << endl;
            informeAuditoria << "<tr><th>ESSID</th><th>BSSID</th><th>Cifrado</th></tr>" << endl;
            for (int i=0;i<row;i++){
                informeAuditoria << "<tr>" << "<td>" << matriz[i][1] << "</td>" << "<td>" << matriz[i][0] << "</td>"<< "<td>" << matriz[i][6] << "</td>" << endl;

            }
            informeAuditoria << "</table>" << endl;

        }
        else{
            vulnCp004 = 1;
            informeAuditoria << "<p>El control ha escaneado las siguientes redes corporativas, detectándose el uso de protocolos de cifrado inseguros en " << detecciones << " redes: </p>" << endl;
            informeAuditoria << "<table>" << endl;
            informeAuditoria << "<tr><th>ESSID</th><th>BSSID</th><th>Cifrado</th></tr>" << endl;
            for (int i=0;i<row;i++){
                if(cifradoInseguro[i][0])
                informeAuditoria << "<tr>" << R"(<td class = "rojo">)" << matriz[i][1] << "</td>" << R"(<td class = "rojo">)" << matriz[i][0] << "</td>"<< R"(<td class = "rojo">)" << matriz[i][6] << "</td>" << endl;
                else
                informeAuditoria << "<tr>" << R"(<td class = "verde">)" << matriz[i][1] << "</td>" << R"(<td class = "verde">)" << matriz[i][0] << "</td>"<< R"(<td class = "verde">)" << matriz[i][6] << "</td>" << endl;
            }
            informeAuditoria << "</table>" << endl;
            informeAuditoria << "<h3>Recomendaciones</h3>" << endl;
            informeAuditoria << "<ul1>" << endl;
            informeAuditoria << "<li>TKIP (Temporal Key Integrity Protocol) y WEP (Wired Equivalent Privacy) son dos protocolos de seguridad de redes inalámbricas obsoletos y altamente inseguros. </p>" << endl;
            informeAuditoria << "<li>Se recomienda dejar de emplearlos, migrando a protocolos de seguridad más robustos, como WPA2, WPA3 o incluso una red con autenticación empresarial si es posible.</p>" << endl;
            informeAuditoria << "</ul1>" << endl;

        }
    }
    //Reporte OWISAM-CF-001
    if (cf001){
        string rutaCf001 = carpetaProyecto + "/OWISAM-CF-001/OWISAM-CF-001.jgm";
        informeAuditoria << R"(<h2 id="cf001">OWISAM-CF-001</h2>
        <h3> Descripción del control </h3>
        <p>El control OWISAM-CF-001 busca redes corporativas que empleen SSIDs genéricos.</p>)" << endl;
        informeAuditoria << "<p>El uso de SSIDs genéricos entraña riesgos, tales como:</p>" << endl;
        informeAuditoria << "<ul1>" << endl;
        informeAuditoria << "<li>Seguridad de la red Wi-Fi: Utilizar SSID genéricos como puede exponer a los usuarios a riesgos de seguridad, ya que otras personas que tengan routers de las mismas marcas pueden conectarse inadvertidamente o de manera intencionada a la red, lo que podría comprometer su seguridad y privacidad.</li>" << endl;
        informeAuditoria << "<li>Dificultad para identificar la red: En áreas urbanas densamente pobladas, múltiples redes Wi-Fi pueden llevar el mismo SSID genérico, lo que dificulta que los usuarios identifiquen su propia red y pueden conectarse accidentalmente a la red equivocada.</li>" << endl;
        informeAuditoria << "<li>Facilita ataques de phishing: Los ciberdelincuentes pueden aprovechar SSID genéricos para llevar a cabo ataques de phishing. Pueden configurar una red Wi-Fi falsa con un SSID común y engañar a las personas para que se conecten a ella, lo que les permite robar información personal o llevar a cabo otros tipos de ataques.</li>" << endl;
        informeAuditoria << "<li>Vulnerabilidades conocidas: Los SSID genéricos suelen estar asociados con modelos populares de routers de consumo. Si un atacante conoce el SSID y el modelo del router, puede buscar en línea vulnerabilidades conocidas específicas de ese modelo y realizar ataques dirigidos.</li>" << endl;
        informeAuditoria << "<li>Dificultad para solucionar problemas: Si los usuarios necesitan ayuda con su red y tienen un SSID genérico, puede ser más difícil para los técnicos de soporte o amigos identificar y ayudar con problemas específicos de la red, ya que muchos usuarios pueden tener el mismo SSID.</li>" << endl;
        informeAuditoria << "</ul1>" << endl;
        ifstream file(rutaCf001);
        string line;
        string matriz[maxFilas][maxColumnas];
        getline(file,line);
        int row = 0;
        int cols = 0;
        while (getline(file, line)) {
            string token;
            istringstream tokenStream(line);
            int col = 0;

            while (getline(tokenStream, token, ',')) {
                matriz[row][col] = token;
                col++;
            }

            cols = std::max(cols, col);
            row++;
        }
        file.close();
        informeAuditoria << "<h3>Resultados</h3>" << endl;
        if (row == 0)
            informeAuditoria << "<p>No se han encontrado redes corporativas que usen SSIDs genéricos.</p>" << endl;
        else{
            vulnCf001 = 1;
            informeAuditoria << "<p>Se han encontrado " << row << " redes que emplean SSID genérico, las cuales exponemos a continuación:" << endl;
            informeAuditoria << "<table>" << endl;
            informeAuditoria << "<tr><th>BSSID</th><th>ESSID</th></tr>" << endl;
            for (int i=0; i<row ; i++){
                informeAuditoria << "<tr>" << endl;
                informeAuditoria << "<td>" << matriz[i][0] << "</td>" << "<td>" << matriz[i][1] << "</td>" << endl;
                informeAuditoria << "</tr>" << endl;
            }
            informeAuditoria << "</table>" << endl;
            informeAuditoria << "<h3>Recomendaciones</h3>" << endl;
            informeAuditoria << "<p>Se recomienda personalizar el nombre de la red Wi-Fi (SSID) de manera única y no reveladora de información personal.Esto mejorará la seguridad de la red, dificultará que otros se conecten sin autorización y facilitará la identificación de la red propia, reduciendo así los riesgos de seguridad y privacidad asociados con SSID genéricos.</p>" << endl;

        }
    }
    //Reporte OWISAM-IF-003
    if (if003){
        string rutaIf001 = carpetaProyecto + "/OWISAM-IF-003/OWISAM_IF_003.jgm";
        informeAuditoria << R"(<h2 id="if003">OWISAM-IF-003</h2>
        <h3> Descripción del control </h3>
        <p>El control OWISAM-IF-003 busca host accesibles en rangos hacia los que no se debería de poder navegar para detectar posibles malas configuraciones del firewall.</p>)" << endl;
        ifstream file(rutaIf001);
        string line;
        string matriz[maxFilas][maxColumnas];
        getline(file,line);
        if(line=="0"){
            informeAuditoria << "<h3>Resultados</h3>" << endl;
            informeAuditoria << "<p>No se han encontrado hosts accesibles en rangos no autorizados.</p>";
        }
        else{
            vulnIf003 = 1;
            int row = 0;
            int cols = 0;
            while (getline(file, line)) {
                string token;
                istringstream tokenStream(line);
                int col = 0;

                while (getline(tokenStream, token, ',')) {
                    matriz[row][col] = token;
                    col++;
                }

                cols = std::max(cols, col);
                row++;
            }
            file.close();
            informeAuditoria << "<h3>Resultados</h3>" << endl;
            if (row == 0)
                informeAuditoria << "<p>Se han encontrado los siguientes hosts accesibles en rangos que no se deberían de alcanzar:</p>" << endl;
            else{
                informeAuditoria << "<table>" << endl;
                informeAuditoria << "<tr><th>Dirección IP</th></tr>" << endl;
                for (int i=0; i<row ; i++){
                    informeAuditoria << "<tr>" << endl;
                    informeAuditoria << "<td>" << matriz[i][0] << "</td>" << endl;
                    informeAuditoria << "</tr>" << endl;
                }
                informeAuditoria << "</table>" << endl;
                informeAuditoria << "<h3>Recomendaciones</h3>" << endl;
                informeAuditoria << "<ul2><li>Antes de configurar las reglas del firewall, es importante que la persona encargada entienda las necesidades de segmentación de la red. Esto implica definir qué tipos de tráfico deben permitirse entre los diferentes segmentos y cuál debe bloquearse.</li>" << endl;
                informeAuditoria << "<li>Se debe dividir la red en segmentos lógicos según las necesidades de la organización. Los segmentos comunes incluyen la red de invitados, la red de empleados y la red de servidores.</li>" << endl;
                informeAuditoria << "<li>Es necesario crear reglas de firewall que permitan o bloqueen el tráfico entre los diferentes segmentos de red. Por ejemplo, se puede permitir que la red de invitados acceda a Internet pero bloquear el acceso a recursos internos.</li>" << endl;
                informeAuditoria << "</ul2>" << endl;

            }
        }
    }
    //Reporte OWISAM-DS-001
    if (ds001){
        string rutaDs001 = carpetaProyecto + "/OWISAM-DS-001/OWISAM_DS_001.jgm";
        informeAuditoria << R"(<h2 id="ds001">OWISAM-DS-001</h2>
        <h3> Descripción del control </h3>
        <p>El control OWISAM-DS-001 busca redes vulnerables a ataques de denegación de servicio</p>)" << endl;
        ifstream file(rutaDs001);
        string line;
        string matriz[maxFilas][maxColumnas];
        int row = 0;
            int cols = 0;
            while (getline(file, line)) {
                string token;
                istringstream tokenStream(line);
                int col = 0;

                while (getline(tokenStream, token, ',')) {
                    matriz[row][col] = token;
                    col++;
                }

                cols = std::max(cols, col);
                row++;
            }
            file.close();
        if(matriz[0][0]=="0"){
            informeAuditoria << "<h3>Resultados</h3>" << endl;
            informeAuditoria << "<p>No se han encontrado redes vulnerables a ataques de denegación de servicio.</p>";
        }
        else{
            vulnDs001 = 1;
            informeAuditoria << "<h3>Resultados</h3>" << endl;
            informeAuditoria << "<p>Se han encontrado las siguientes redes vulnerables a ataques de denegación de servicio:</p>" << endl;
                informeAuditoria << "<table>" << endl;
                informeAuditoria << "<tr><th>BSSID</th><th>ESSID</th></tr>" << endl;
                for (int i=0; i<row ; i++){
                    informeAuditoria << "<tr>" << endl;
                    informeAuditoria << "<td>" << matriz[i][0] << "</td>" << "<td>" << matriz[i][1] << "</td>" << endl;
                    informeAuditoria << "</tr>" << endl;
                }
                informeAuditoria << "</table>" << endl;
                informeAuditoria << "<h3>Recomendaciones</h3>" << endl;
                informeAuditoria << "<p>Para proteger una red Wi-Fi de ataques de denegación de servicio (DoS) y para incluir un Sistema de Detección de Intrusiones (IDS) específicamente para Wi-Fi, se dan las siguientes recomendaciones:</p>" << endl;
                informeAuditoria << "<ul2><li>Actualizar y Parchear el Firmware y el Software del punto de acceso.</li>" << endl;
                informeAuditoria << "<li>Implementa un Sistema de Detección de Intrusiones (IDS) o un Sistema de Prevención de Intrusiones (IPS) específicamente diseñado para redes Wi-Fi. Algunos dispositivos de seguridad, como algunos modelos de puntos de acceso empresariales, pueden tener esta funcionalidad incorporada..</li>" << endl;
                informeAuditoria << "<li>Configurar el IDS/IPS para detectar comportamientos inusuales en la red Wi-Fi, como un alto número de intentos de autenticación fallidos o actividad anormal.</li>" << endl;
                informeAuditoria << "</ul2>" << endl;

        }
    }
	//Reporte OWISAM-CT-001
    if (ct001){
        string rutaDs001 = carpetaProyecto + "/OWISAM-CT-001/OWISAM_CT_001.jgm";
        informeAuditoria << R"(<h2 id="ct001">OWISAM-CT-001</h2>
        <h3> Descripción del control </h3>
        <p>El control OWISAM-CT-001 es un ataque activo cuyo objetivo es capturar credenciales suplantando puntos de acceso.</p>)" << endl;
        ifstream file(rutaDs001);
        string line;
        string matriz[maxFilas][maxColumnas];
        int row = 0;
            int cols = 0;
            while (getline(file, line)) {
                string token;
                istringstream tokenStream(line);
                int col = 0;

                while (getline(tokenStream, token, ',')) {
                    matriz[row][col] = token;
                    col++;
                }

                cols = std::max(cols, col);
                row++;
            }
            file.close();
        if(matriz[0][0]=="SIN HALLAZGOS"){
            informeAuditoria << "<h3>Resultados</h3>" << endl;
            informeAuditoria << "<p>No se han comprometido credenciales de redes corporativas.</p>";
        }
        else{
            vulnCt001 = 1;
            informeAuditoria << "<h3>Resultados</h3>" << endl;
            informeAuditoria << "<p>Se han capturado credenciales de las siguientes redes:</p>" << endl;
                informeAuditoria << "<table>" << endl;
                informeAuditoria << "<tr><th>ESSID</th><th>Contraseña</th></tr>" << endl;
                for (int i=0; i<row ; i++){
                    informeAuditoria << "<tr>" << endl;
                    informeAuditoria << "<td>" << matriz[i][0] << "</td>" << "<td>" << matriz[i][1] << "</td>" << endl;
                    informeAuditoria << "</tr>" << endl;
                }
                informeAuditoria << "</table>" << endl;
                informeAuditoria << "<h3>Recomendaciones</h3>" << endl;
                informeAuditoria << "<ul2><li>Cambio de las credenciales de forma periódica.</li>" << endl;
                informeAuditoria << "<li>Establecer una política de contraseñas que cumpla con las directrices de contraseñas seguras establecidad por organismos de referencia como el CCN o Incidbe.</li>" << endl;
                informeAuditoria << "</ul2>" << endl;

        }
    }
    //Generar resumen ejecutivo
    informeAuditoria << R"(<h2 id="resumen">Resumen ejecutivo</h2>
        <p>Durante el proceso de auditoría realizado con fecha )" << fecha_formateada << " por el auditor " << nombreAuditor << " de la empresa " << empresaAuditora << " para " << empresaAuditada << " se han detectado las siguientes vulnerabilidades: </p>" << endl;
    informeAuditoria << "<table>" << endl;
    informeAuditoria << "<tr><th>CWSS</th><th>Código</th><th>Vulnerabilidad</th></tr>" << endl;
    if(vulnFp004){
        informeAuditoria << "<tr>" << R"(<td class = "rojo">10</td><td>OWISAM-FP-004</td><td>Rogue APs detectados</td></tr>)";
    }
    if(vulnIf003){
        informeAuditoria << "<tr>" << R"(<td class = "rojo">9</td><td>OWISAM-IF-003</td><td>Acceso a segmentos de red restringidos</td></tr>)";
    }
    if(vulnCp004){
        informeAuditoria << "<tr>" << R"(<td>8</td><td>OWISAM-CP-004</td><td>Debilidad de seguridad en la red</td></tr>)";
    }
    if(vulnAu004){
        informeAuditoria << "<tr>" << R"(<td>7</td><td>OWISAM-AU-004</td><td>Credenciales débiles</td></tr>)";
    }
    if(vulnCt001){
        informeAuditoria << "<tr>" << R"(<td>6</td><td>OWISAM-CT-001</td><td>Robo de credenciales con RogueAP</td></tr>)";
    }
	if(vulnDs001){
        informeAuditoria << "<tr>" << R"(<td>6</td><td>OWISAM-DS-001</td><td>Interceptación de credenciales de autenticación</td></tr>)";
    }
    if(vulnDi002){
        informeAuditoria << "<tr>" << R"(<td>5</td><td>OWISAM-DI-002</td><td>Redes ocultas detectadas</td></tr>)";
    }
    if(vulnCf001){
        informeAuditoria << "<tr>" << R"(<td>4</td><td>OWISAM-CF-001</td><td>Redes wireless con ESSID genérico</td></tr>)";
    }
    informeAuditoria << "</table>" << endl;
}

void MenuOpciones(){
    system("clear");
    int input;
    bool cierre = 0;
    while(cierre == 0){
        cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
        cout << "[OPCIONES]" << endl;
        cout << "[1] Configuración de red" << endl;
        cout << "[2] Atrás" << endl;
        // Validar entrada usuario
        while (!(cin >> input)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Introduzca un número: ";
        }
        switch(input){
            case 1:
                while(ConfiguracionDeRed()==0)
                    ConfiguracionDeRed();
            break;
            case 2:
                cierre = 1;
            default:
                cout << "Opción no válida" << endl;
            break;
            }
        }
    }

void OWISAM_CT(){

    system("clear");
    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl << endl;
    cout << "Los controles del a categoría OWISAM-CT buscan auditar los clientes inalámbricos para buscar y explotar vulnerabilidades en ellos que no necesariamente afecten al entorno WIFI, por tanto se excluyen del alcance de este proyecto." << endl;
    cout << "No obstante, en está sección se incluyen varias utilidades que podrían ser útiles a la hora de auditarlo:" << endl;
    cout << endl << endl << "[+] Captura de hansake WPA" << endl;
    cout << "            El comando sudo wifite -v escaneará el entorno Wifi en la interfaz por defecto, permitiéndonos seleccionar una red sobre la que lanzar la captura del hanshake. Además tratará de descifrar el hash de la contraseña por diccionario" << endl;
    cout << "[+] Creación de RogueAP" << endl;
    cout << "            Un rogueAP es un punto de acceso falso que suplanta a uno legítimo. Puede ser de utilidad en ataques MiTM y muchos otros. Se puede crear facilimente con el comando: sudo airbase-ng -a <BSSID del rogueAP> -e <ESSID del rogueAP> <interfaz de red>" << endl;
    cout << "[+] Desautenticación de un cliente inalámbrico" << endl;
    cout << "            Puede ser de utilidad en varios ataques. Una forma sencilla es mediante el comando sudo aireplay-ng -0 1000 -a <BSSID del AP> -c <MAC del cliente> <Interfaz de red>" << endl;
    cout << endl << endl << "[Introduce cualquier entrada para volver atrás.]" << endl;
    string input;
    cin >> input;
}

void OWISAM_CT_001(string carpetaProyecto){
	system("clear");
	cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
	cout << "[Control OWISAM-CT-001]" << endl << endl;
	string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
	int passCapturadas = 0;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    string carpetaControl = carpetaProyecto + "/OWISAM-CT-001" ;
    ofstream ficheroSalida(carpetaControl + "/OWISAM_CT_001.jgm");
    cout << "Creando carpeta " << carpetaControl << endl;
    string comando = "mkdir " + carpetaControl;
    system(comando.c_str());
	cout << "El control OWISAM-CT-001 pretende el robo de credenciales creando un RogueAP que suplante a un punto de acceso legítimo." << endl;
	cout << "Para ello, OWISAM-AAHT crea un punto de acceso automáticamente, y el auditor debe capturar el intercambio WPA en otra interfaz y crackear el hash capturado." << endl;
	sleep(10);
	bool fin = 0;
	while(!fin){
		system("clear");
		cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
		cout << "[Control OWISAM-CT-001]" << endl << endl;
		cout << "Creando RogueAP." << endl << endl;
		cout << "Introduce el BSSID del AP a suplantar:";
		string bssid;
		cin >> bssid;
		cout << endl << "Introduce el ESSID del AP a suplantar:";
		string essid;
		cin >> essid;
		cout << endl << "Introduce la interfaz de red en la que desplegar el RogueAP:";
		string interfazRed;
		cin >> interfazRed;
		cout << endl << endl << "Creando RogueAP:" << "ESSID:" << essid << ", BSSID:" << bssid << ", Interfaz:" << interfazRed << endl;
		string comando = "sudo airbase-ng -a " + bssid + " -e " + essid + " " + interfazRed;
		cout << "CTRL+C para finalizar, una vez hayas capturado el handshake.";
		cout << "Lanzando comando: " << comando << endl;
		system(comando.c_str());
		cout << endl << "¿Se ha conseguido romper la clave?[s/n]";
		string respuesta;
		cin >> respuesta;
		if(respuesta == "s"){
				cout << endl << "¿Cual es la contraseña?";
				cin >> respuesta;
				cout << endl << "Se exportara en el informe" << endl;
				ficheroSalida << essid << "," << respuesta << endl;
				passCapturadas += 1;
		}
		cout << "¿Deseas suplantar otros AP?[s/n]";
		cin >> respuesta;
		if(respuesta == "n")
			fin = 1;

	}
	if (passCapturadas == 0){
				ficheroSalida << "SIN HALLAZGOS";
		}
}

void OWISAM_DS_001(string carpetaProyecto){

    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << "[Control OWISAM-DS-001]" << endl << endl;
    string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    bool fin = 0, redesVulnerables = 0;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    string carpetaControl = carpetaProyecto + "/OWISAM-DS-001" ;
    ofstream ficheroSalida(carpetaControl + "/OWISAM_DS_001.jgm");
    cout << "Creando carpeta " << carpetaControl << endl;
    string comando = "mkdir " + carpetaControl;
    system(comando.c_str());
    cout << "El control OWISAM-DI-001 busca redes vulnerables ante ataques de Denegación de Servicio por medio del envío de paquetes de desautenticación al AP" << endl;
    cout << "Para ello, se debe especificar el BSSID, ESSID del AP a atacar, así como el canal en el que emite y la MAC del cliente que se desea desautenticar" << endl;
    cout << "Aunque ver si el AP devuelve paquetes ACK a nuestras peticiones es un buen indicador para observar si el ataque tiene éxito, recomendamos tener acceso a un cliente conectado a la red para comprobar si se pierde la conexión." << endl;
    cout << endl << endl;
    sleep(10);
    while (!fin){
        string bssidAp, essidAp, macCliente, canal;
        cout << "Introduce el BSSID de la red a auditar:";
        cin >> bssidAp;
        cout << endl << "Introduce el ESSID de la red a auditar:";
        cin >> essidAp;
        cout << endl << "Introduce el canal en el que emite:";
        cin >> canal;
        cout << endl << "Introduce la MAC del cliente a desautenticar:";
        cin >> macCliente;
        cout << endl << endl << "Comenzando ataque DoS contra el cliente " << macCliente << " en la red " << essidAp << "[Ctrl + C para finalizar]" << endl;
        // Fijar la interfaz monitor en el canal adecuado
        comando = "sudo iwconfig " + interfazCaptura + " channel 1";
		cout << "Lanzando comando: " << comando << endl;
		cout << "El comando sirve para fijar la interfaz de red en un canal determinado, en este caso el canal: " << canal << endl;
		sleep(3);
        system(comando.c_str());
        comando = "sudo aireplay-ng -0 1000 -a " + bssidAp + " -c " + macCliente + " " + interfazCaptura;
		cout << endl << "Lanzando comando: " << comando << endl;
		cout << "El comando se usa para enviar 1000 peticiones de desautencicación del cliente al AP." << endl;
		sleep(3);
        system(comando.c_str());
        cout << "¿Se ha conseguido desautenticar el cliente? (s/n)";
        string respuesta;
        cin >> respuesta;
        cout << endl;
        if(respuesta == "s"){
            redesVulnerables = 1;
            ficheroSalida << bssidAp << "," << essidAp << endl;
            cout << "¿Deseas auditar otra red? (s/n)";
            cin >> respuesta;
            cout << endl;
            if (respuesta == "n"){
                fin = 1;
                if(redesVulnerables == 0)
                    ficheroSalida << "0";
            }
        }
        else{
            cout << "¿Deseas auditar otra red? (s/n)";
            cin >> respuesta;
            cout << endl;
            if (respuesta == "n") {
                fin = 1;
                if(redesVulnerables = 0)
                    ficheroSalida << "0";
            }
        }
    }
}

void OWISAM_IF_003(string carpetaProyecto){

    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << "[Control OWISAM-IF-003]" << endl << endl;
    string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    string carpetaControl = carpetaProyecto + "/OWISAM-IF-003" ;
    cout << "Creando carpeta " << carpetaControl << endl;
    string comando = "mkdir " + carpetaControl;
    system(comando.c_str());
    cout << "El control OWISAM-IF-003 escanea el entorno de red buscando dispositivos accesibles en rangos que no deberían de estar permitidos." << endl;
    cout << "Por defecto, escanea todos los segmentos de red privados:" << endl;
    cout << endl << "      [+] Clase A: 10.0.0.0/8" << endl;
    cout << "      [+] Clase B: 172.16.0.0/12" << endl;
    cout << "      [+] Clase C: 192.168.0.0/16" << endl;
    cout << endl <<  "Por tanto, el escaneo puede demorarse bastante. ¿Deseas establecer tu los rangos a escanear?(s/n):";
    string respuesta;
    bool finLoop = 0, finLoop1 = 0, finLoop2 =0;
    string rangosEscaneo[10];
    int numRangos = 0;
    while(!finLoop){
    	cin >> respuesta;
    	if(respuesta == "s" || respuesta == "n"){
    		if(respuesta == "n"){
    			finLoop = 1;
    		}
    		else{
    			while(!finLoop1){
	    			if(numRangos <= 9){
	    				cout << endl << "Introduce el " << numRangos + 1 << "º rango a escanear:";
	    				cin >> rangosEscaneo[numRangos];
	    				numRangos +=1;
	    				cout << endl << "Deseas escanear mas rangos [maximo 10 rangos](s/n):";
	    				finLoop2 = 0;
	    				while(!finLoop2){
	    					cin >> respuesta;
	    					if(respuesta == "s" || respuesta == "n"){
	    						if(respuesta == "n"){
	    							finLoop = 1;
	    							finLoop1 = 1;
	    							finLoop2 = 1;
	    						}
	    						else{
	    							finLoop2 = 1;
	    						}
	    					}
	    					else{
	    						cout << endl << "Respuesta incorrecta, ¿deseas introducir mas rangos a escanear? (s/n)";
	    					}
	    				}
	    			}
	    			else {
	    				cout << "Ya se ha alcanzado el numero maximo de rangos a escanear. Comenzando control..." << endl;
	    				sleep(5);
	    				finLoop = 1;
	    				finLoop1 = 1;
	    			}
    			}
    		}
    	}
    	else{
    		cout << endl << "Respuesta incorrecta, ¿deseas establecer tu los rangos a escanear (contesta con 's' o 'n'):";
    	}
    }
    if(numRangos == 0){
    	comando = "cd " + carpetaControl + " && sudo nmap -sn 10.0.0.0/8 172.16.0.0/12 192.168.0.0/16 -oN salidaNmap.txt";
    }
    else{
    	comando = "cd " + carpetaControl + " && sudo nmap -sn";
    	for(int i=0; i<numRangos; i++){
		comando+= " " + rangosEscaneo[i];
    	}
    	comando +=" -oN salidaNmap.txt";
    	system(comando.c_str());
    }
    comando = "cd " + carpetaControl + " && awk '/Nmap scan report for/{print $5}' salidaNmap.txt";
    system(comando.c_str());
    comando = R"(grep -oE '([0-9]{1,3}\.){3}[0-9]{1,3}' )";
    comando += carpetaControl +"/salidaNmap.txt > " + carpetaControl +"/hosts.txt";
    cout << endl << endl << endl << "Lanzando comando: " << comando << endl;
    system(comando.c_str());
    string rutaHosts = carpetaControl + "/hosts.txt";
    ofstream ficheroSalida(carpetaControl + "/OWISAM_IF_003.jgm");
    int lineas = contarLineas(rutaHosts);
    ifstream ficheroHosts(rutaHosts);
    string hosts[lineas];
    cout << "Se han encontrado los siguientes hosts accesibles:" << endl;
    for (int i=0; i<lineas;i++){
    	getline(ficheroHosts,hosts[i]);
    	if(i==0){
    		if(hosts[i]==""){
    			cout << "No se han encontrado hosts accesibles. Finalizando control" << endl;
    			sleep(5);
    			ficheroSalida << "0";
    			ficheroSalida.close();
    		}
    		else{
    			cout << "[" << i+1 << "] " << hosts[i] << endl;
    		}
    	}
    	else{
    		cout << "[" << i+1 << "] " << hosts[i] << endl;
    	}
    }
    cout << "Introduce cuál de los hosts detectados pertenece a segmentos de red restringidos (separados por ',' o 0 si no hay ninguno):";
    getline(cin, respuesta); // Lee la respuesta del usuario
    while(respuesta.empty())
        getline(cin, respuesta);
    istringstream ss(respuesta);
    string token;
    if (respuesta != "0"){
	    while (getline(ss, token, ',')) {
		int i;
		stringstream converter(token);
		converter >> i;
		ficheroSalida << hosts[i-1] << endl;
	    }
	ficheroSalida.close();
    }
    else{
    	cout << "No se han encontrado hosts accesibles. Finalizando control" << endl;
    	sleep(5);
    	ficheroSalida << 0;
    	ficheroSalida.close();
    }
}

void OWISAM_CF_001(string carpetaProyecto){

    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << "[Control OWISAM-CF-001]" << endl << endl;
    string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    string carpetaControl = carpetaProyecto + "/OWISAM-CF-001" ;
    cout << "Creando carpeta " << carpetaControl << endl;
    string comando = "mkdir " + carpetaControl;
    system(comando.c_str());
    ficheroSSIDGenericos(carpetaControl);
    //sudo airodump-ng -w /home/jorge/Escritorio/ --output-format csv wlan1mon
    cout << "Inicializando escaneo de entorno WIFI (ctrl + C para finalizar)" << endl;
    sleep (10);
    comando = "sudo airodump-ng -w " + carpetaControl + "/" + " --output-format csv " + interfazCaptura;
	cout << endl << "Lanzando comando: " << comando << endl;
	cout << "El comando se emplea para que airodump escanee el entorno y guarde los resultados como un csv." << endl;
	sleep(3);
    int valorSystem;
    valorSystem = system(comando.c_str());
    while(valorSystem != 0){
    }
    string rutaAirodump = carpetaControl + "/-01.csv";
    ifstream archivo(rutaAirodump);
    string matriz[maxFilas][maxColumnas]; // Matriz para almacenar los datos CSV
    string linea;
    int fila = 0;
    while (getline(archivo, linea) && fila < maxFilas) {
        istringstream ss(linea);
        string valor;
        int columna = 0;
        while (getline(ss, valor, ',') && columna < maxColumnas) {
            matriz[fila][columna] = valor;
            columna++;
        }
        fila++;
    }
    archivo.close(); // Cierra el archivo
    // Imprimir la matriz de datos
    bool fin = 0;
        for(int i=1; i< maxFilas; i++){
            if (!fin){
                if (matriz[i][0] == "Station MAC"){
                    fin = 1;
                }
                else{
                    if (i==1)
                        cout << matriz[i][0] << "                   " << matriz[i][13] << endl;
                    else
                        cout << "[" << i-1 << "] " << matriz[i][0] << "" << matriz[i][13] << endl;
                }
            }

        }
    cout << "[OWISAM-CP-004] ¿Cuales de los siguientes redes son redes corporativas de la empresa auditada? (Introdúce los id de cada red separados por coma, ejemplo: 1,4,20 o 0 si no existe ninguno) ---->";
    string respuesta;
    getline(cin, respuesta); // Lee la respuesta del usuario
    while(respuesta.empty())
        getline(cin, respuesta);
    istringstream ss(respuesta);
    string token,rutaControl;
    rutaControl = carpetaControl + "/OWISAM-CF-001.jgm";
    comando = "touch " + rutaControl;
    ofstream ficheroSalida(rutaControl);
    ficheroSalida << "BSSID,ESSID" << endl;
    string rutaSsidGenericos = carpetaControl + "/da795e571fb5f91f9e86a27f2c2f626f/commonssids.txt";
    string lista[5000];
    ifstream ssidGenericos(rutaSsidGenericos);
    for (int i=0; i<4999;i++)
        getline(ssidGenericos,lista[i]);
    ssidGenericos.close();
    int ssidGenerico = 0;
    if (respuesta != "0"){
        while (getline(ss, token, ',')) {
            int i;
            stringstream converter(token);
            converter >> i;
            for (int j=0;j<4999;j++){
                if(lista[j].length()>4){
                    // Verificar si el string contiene un valor específico
                    if (strcasestr(matriz[i+1][13].c_str(),lista[j].c_str())){
                        if (matriz[i+1][13] != ""){
                            ssidGenerico +=1;
                            ficheroSalida << matriz[i+1][0] << "," << matriz[i+1][13] << endl;
                            cout << "Se ha detectado una red corporativa con nombre genérico: " << matriz[i+1][13] << " . En total, se han detectado " << ssidGenerico << "redes con SSID genérico."<< endl;
                            sleep(3);
                        }
                    }
                }
            }
    }
    }
}

void OWISAM_CP_004(string carpetaProyecto){
    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << "[Control OWISAM-CP-004]" << endl << endl;
    string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    string carpetaControl = carpetaProyecto + "/OWISAM-CP-004" ;
    cout << "Creando carpeta " << carpetaControl << endl;
    string comando = "mkdir " + carpetaControl;
    system(comando.c_str());
    //sudo airodump-ng -w /home/jorge/Escritorio/ --output-format csv wlan1mon
    cout << "Inicializando escaneo de entorno WIFI (ctrl + C para finalizar)" << endl;
    sleep (10);
    comando = "sudo airodump-ng -w " + carpetaControl + "/" + " --output-format csv " + interfazCaptura;
	cout << endl << "Lanzando comando: " << comando << endl;
	cout << "El comando se emplea para que airodump escanee el entorno y guarde los resultados como un csv." << endl;
	sleep(3);
    int valorSystem;
    valorSystem = system(comando.c_str());
    while(valorSystem != 0){
    }
    string rutaAirodump = carpetaControl + "/-01.csv";
    ifstream archivo(rutaAirodump);
    string matriz[maxFilas][maxColumnas]; // Matriz para almacenar los datos CSV
    string linea;
    int fila = 0;
    while (getline(archivo, linea) && fila < maxFilas) {
        istringstream ss(linea);
        string valor;
        int columna = 0;
        while (getline(ss, valor, ',') && columna < maxColumnas) {
            matriz[fila][columna] = valor;
            columna++;
        }
        fila++;
    }
    archivo.close(); // Cierra el archivo
    // Imprimir la matriz de datos
    bool fin = 0;
        for(int i=1; i< maxFilas; i++){
            if (!fin){
                if (matriz[i][0] == "Station MAC"){
                    fin = 1;
                }
                else{
                    if (i==1)
                        cout << matriz[i][0] << "                   " << matriz[i][13] << endl;
                    else
                        cout << "[" << i-1 << "] " << matriz[i][0] << "" << matriz[i][13] << endl;
                }
            }

        }
    cout << "[OWISAM-CP-004] ¿Cuales de los siguientes redes son redes corporativas de la empresa auditada? (Introdúce los id de cada red separados por coma, ejemplo: 1,4,20 o 0 si no existe ninguno) ---->";
    string respuesta;
    getline(cin, respuesta); // Lee la respuesta del usuario
    while(respuesta.empty())
        getline(cin, respuesta);
    istringstream ss(respuesta);
    string token,rutaControl;
    rutaControl = carpetaControl + "/OWISAM-CP-004.jgm";
    comando = "touch " + rutaControl;
    ofstream ficheroSalida(rutaControl);
    ficheroSalida << "BSSID,ESSID,Vista por primera vez,Canal,Velocidad,Privacidad,Cifrado,Autenticacion,Potencia,Balizas" << endl;
    if (respuesta != "0"){
    while (getline(ss, token, ',')) {
        int i;
        stringstream converter(token);
        converter >> i;
            cout << "Escribiendo [" << i << "]" << matriz[i+1][0] << "," << matriz[i+1][13] << "," << matriz[i+1][1] << "," << matriz[i+1][3] << "," << matriz[i+1][4] << "," << matriz[i+1][5] << "," << matriz[i+1][6] << "," << matriz[i+1][7] << "," << matriz[i+1][8] << "," << matriz[i+1][9] << endl;
            ficheroSalida << matriz[i+1][0] << "," << matriz[i+1][13] << "," << matriz[i+1][1] << "," << matriz[i+1][3] << "," << matriz[i+1][4] << "," << matriz[i+1][5] << "," << matriz[i+1][6] << "," << matriz[i+1][7] << "," << matriz[i+1][8] << "," << matriz[i+1][9] << endl;
    }
    ficheroSalida.close();

    }
}

void OWISAM_AU_004(string carpetaProyecto){
    system("clear");
    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << "[Control OWISAM-AU-004]" << endl << endl;
    string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    string carpetaControl = carpetaProyecto + "/OWISAM-AU-004" ;
    cout << "Creando carpeta " << carpetaControl << endl;
    string comando = "mkdir " + carpetaControl;
    system(comando.c_str());
    system("clear");
    cout << "Inicializando control. Se va a emplear la herramienta Wifite para capturar el handshake WPA de las redes corporativas." << endl;
    cout << "Instrucciones: Detener el escaneo con ctrl + C una vez se hayan detectado todas las redes a auditar. Posteriormente, introducir el ID de la/s red/es a escanear." << endl;
    cout << "Automáticamente, Wifite capturará los handshakes de la redes seleccionadas e intentará crear los hashes de capturados." << endl;
    sleep(20);
    system("clear");
    comando = "cd " +carpetaControl + " && sudo wifite -v";
    system(comando.c_str());
    string rutaFicheroCracked = carpetaControl + "/cracked.json", rutaFicheroSalida = carpetaControl + "/OWISAM-AU-004.jgm";
    ifstream ficheroCracked(rutaFicheroCracked);
    ofstream ficheroSalida(rutaFicheroSalida);
    if (ficheroCracked){
        int cantidadDeLineas = contarLineas(rutaFicheroCracked),lineaActual = 0;
        cout << "Se han detectado credenciales débiles en redes corporativas. Se anexan al informe de auditoría." << endl;
        sleep(10);
        ficheroSalida << "ESSID,BSSID,Contraseña,Seguridad" << endl;
        string linea,ESSID,BSSID,pass,sec;
        //getline(ficheroCracked,linea);
        //getline(ficheroCracked,linea);
        while (std::getline(ficheroCracked, linea)) {
            size_t posicionDosPuntos = linea.find(":");
            if (posicionDosPuntos != std::string::npos) {
                // Extraer todo lo que está después de los dos puntos
                if (lineaActual == 2){
                std::string valor = linea.substr(posicionDosPuntos + 1);
                sec = valor;
                }
                if (lineaActual == 4){
                std::string valor = linea.substr(posicionDosPuntos + 1);
                ESSID = valor;
                }
                if (lineaActual == 5){
                std::string valor = linea.substr(posicionDosPuntos + 1);
                BSSID = valor;
                }
                if (lineaActual == 6){
                std::string valor = linea.substr(posicionDosPuntos + 1);
                pass = valor;
                }
            }
            lineaActual++;
            if (lineaActual == 8){
                ficheroSalida << ESSID << "," << BSSID << "," << pass << "," << sec << endl;
                lineaActual = 0;
            }
        }
    }
    else{
        cout << endl << endl << "No se han descifrado credenciales inseguras." << endl;
        sleep(10);
        ficheroSalida << "0" << endl;
    }

}

void OWISAM_DI_002(string carpetaProyecto){
    system("clear");
    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << "[Control OWISAM-DI-002]" << endl << endl;
    bool hayRedesOcultas = 0;
    string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    string carpetaControl = carpetaProyecto + "/OWISAM-DI-002" ;
    cout << "Creando carpeta " << carpetaControl << endl;
    string comando = "mkdir " + carpetaControl;
    system(comando.c_str());
    //sudo airodump-ng -w /home/jorge/Escritorio/ --output-format csv wlan1mon
    cout << "Inicializando escaneo de entorno WIFI (ctrl + C para finalizar)" << endl;
    sleep (10);
    comando = "sudo airodump-ng -w " + carpetaControl + "/" + " --output-format csv " + interfazCaptura;
	cout << endl << "Lanzando comando: " << comando << endl;
	cout << "El comando se emplea para que airodump escanee el entorno y guarde los resultados como un csv." << endl;
	sleep(3);
    int valorSystem;
    valorSystem = system(comando.c_str());
    while(valorSystem != 0){
    }
    string rutaAirodump = carpetaControl + "/-01.csv";
    ifstream archivo(rutaAirodump);
    string matriz[maxFilas][maxColumnas]; // Matriz para almacenar los datos CSV
    string linea;
    int fila = 0;
    while (getline(archivo, linea) && fila < maxFilas) {
        istringstream ss(linea);
        string valor;
        int columna = 0;
        while (getline(ss, valor, ',') && columna < maxColumnas) {
            matriz[fila][columna] = valor;
            columna++;
        }
        fila++;
    }
    archivo.close(); // Cierra el archivo
    string rutaControl;
    rutaControl = carpetaControl + "/OWISAM-DI-002.jgm";
    comando = "touch " + rutaControl;
    ofstream ficheroSalida(rutaControl);
    bool fin = 0;
    ficheroSalida << "BSSID,SSID,Vista por primera vez,Canal,Velocidad,Privacidad,Cifrado,Autenticacion,Potencia,Balizas" << endl;
        for(int i=1; i< maxFilas; i++){
            if (!fin){
                if (matriz[i][0] == "Station MAC"){
                    fin = 1;
                }
                else{
                    if(matriz[i][13] == " " && matriz[i][0] != ""){
                        hayRedesOcultas = 1;
                        cout << "Se ha encontrado una red oculta: " << matriz [i][0] << endl;
                        sleep(10);
                        ficheroSalida << matriz[i][0] << "," << matriz[i][13] << "," << matriz[i][1] << "," << matriz[i][3] << "," << matriz[i][4] << "," << matriz[i][5] << "," << matriz[i][6] << "," << matriz[i][7] << "," << matriz[i][8] << "," << matriz[i][9] << endl;

                    }
                }
            }
        }
    if (!hayRedesOcultas){
        cout << "No se han encontrado redes ocultas." << endl;
        sleep(10);
        ficheroSalida << "NO HAY REDES OCULTAS" << endl;

    }
    ficheroSalida.close();
}
void OWISAM_FP_004(string carpetaProyecto){

    string rutaDatosNP1,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
    string carpetaControl = carpetaProyecto + "/OWISAM-FP-004" ;
    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << "[Control OWISAM-FP-004]" << endl << endl;
    cout << "Creando carpeta " << carpetaControl << endl;
    string comando = "mkdir " + carpetaControl;
    system(comando.c_str());
    //sudo airodump-ng -w /home/jorge/Escritorio/ --output-format csv wlan1mon
    cout << "Inicializando escaneo de entorno WIFI (ctrl + C para finalizar)" << endl;
    sleep (10);
    comando = "sudo airodump-ng -w " + carpetaControl + "/" + " --output-format csv " + interfazCaptura;
    int valorSystem;
    valorSystem = system(comando.c_str());
    while(valorSystem != 0){
    }
    string rutaAirodump = carpetaControl + "/-01.csv";
    ifstream archivo(rutaAirodump);
    string matriz[maxFilas][maxColumnas]; // Matriz para almacenar los datos CSV
    string linea;
    int fila = 0;
    while (getline(archivo, linea) && fila < maxFilas) {
        istringstream ss(linea);
        string valor;
        int columna = 0;
        while (getline(ss, valor, ',') && columna < maxColumnas) {
            matriz[fila][columna] = valor;
            columna++;
        }
        fila++;
    }
    archivo.close(); // Cierra el archivo
    // Imprimir la matriz de datos
    bool fin = 0;
        for(int i=1; i< maxFilas; i++){
            if (!fin){
                if (matriz[i][0] == "Station MAC"){
                    fin = 1;
                }
                else{
                    if (i==1)
                        cout << matriz[i][0] << "                   " << matriz[i][13] << endl;
                    else
                        cout << "[" << i-1 << "] " << matriz[i][0] << "" << matriz[i][13] << endl;
                }
            }

        }
    cout << "¿Existe algún dispositivo wifi no autorizado en la lista? (Introdúce los dispositivos separados por coma, ejemplo: 1,4,20 o 0 si no existe ninguno) ---->";
    string respuesta;
    getline(cin, respuesta); // Lee la respuesta del usuario
    while(respuesta.empty())
        getline(cin, respuesta);
    istringstream ss(respuesta);
    string token,rutaControl;
    rutaControl = carpetaControl + "/OWISAM-FP-004.jgm";
    comando = "touch " + rutaControl;
    ofstream ficheroSalida(rutaControl);
    ficheroSalida << "BSSID,ESSID,Vista por primera vez,Canal,Velocidad,Privacidad,Cifrado,Autenticacion,Potencia,Balizas" << endl;
    if (respuesta != "0"){
    while (getline(ss, token, ',')) {
        int i;
        stringstream converter(token);
        converter >> i;
            cout << "Escribiendo [" << i << "]" << matriz[i+1][0] << "," << matriz[i+1][13] << "," << matriz[i+1][1] << "," << matriz[i+1][3] << "," << matriz[i+1][4] << "," << matriz[i+1][5] << "," << matriz[i+1][6] << "," << matriz[i+1][7] << "," << matriz[i+1][8] << "," << matriz[i+1][9] << endl;
            ficheroSalida << matriz[i+1][0] << "," << matriz[i+1][13] << "," << matriz[i+1][1] << "," << matriz[i+1][3] << "," << matriz[i+1][4] << "," << matriz[i+1][5] << "," << matriz[i+1][6] << "," << matriz[i+1][7] << "," << matriz[i+1][8] << "," << matriz[i+1][9] << endl;
    }
    ficheroSalida.close();

    }
}
void PantallaAuditoria(string carpetaProyecto){
    bool fin = 0;
    while (!fin){
        system("clear");
        cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
        cout << "[MENU DE AUDITORIA]" << endl << endl;
        string rutaDatosNP1,linea,lineaLeida,nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
        rutaDatosNP1 = carpetaProyecto + "/datosNP1.jgm";
        ifstream datosNP1(rutaDatosNP1);
        for (int i=0; i<6 ; i++){
            getline(datosNP1,lineaLeida);
        }
        getline(datosNP1,rutaProyecto);
        getline(datosNP1,carpetaProyecto);
        getline(datosNP1,nombreProyecto);
        getline(datosNP1,nombreAuditor);
        getline(datosNP1,empresaAuditora);
        getline(datosNP1,empresaAuditada);
        cout << "Nombre del proyecto: " << "\033[33m" << nombreProyecto << "\033[0m" << endl;
        cout << "Auditor: " << "\033[33m" << nombreAuditor << "\033[0m" << endl;
        cout << "Empresa auditora: " << "\033[33m" << empresaAuditora << "\033[0m" << endl;
        cout << "Empresa auditada: " << "\033[33m" << empresaAuditada << "\033[0m" << endl << endl;
        cout << "CONTROLES AUDITADOS" << endl;
        bool controlAuditado = 0, fp004 = 0,di002 = 0,au004 = 0,cp004 = 0,cf001 = 0,if003 = 0,ds001 = 0,ct001 = 0;
        int respuesta;
        getline(datosNP1,lineaLeida);
        getline(datosNP1,lineaLeida);
        if(lineaLeida == "1"){
            fp004 = 1;
            controlAuditado = 1;
        }
        getline(datosNP1,lineaLeida);
        getline(datosNP1,lineaLeida);
        if(lineaLeida == "1"){
            di002 = 1;
            controlAuditado = 1;
        }
        getline(datosNP1,lineaLeida);
        getline(datosNP1,lineaLeida);
        if(lineaLeida == "1"){
            au004 = 1;
            controlAuditado = 1;
        }
        getline(datosNP1,lineaLeida);
        getline(datosNP1,lineaLeida);
        if(lineaLeida == "1"){
            cp004 = 1;
            controlAuditado = 1;
        }
        getline(datosNP1,lineaLeida);
        getline(datosNP1,lineaLeida);
        if(lineaLeida == "1"){
            cf001 = 1;
            controlAuditado = 1;
        }
        getline(datosNP1,lineaLeida);
        getline(datosNP1,lineaLeida);
        if(lineaLeida == "1"){
            if003 = 1;
            controlAuditado = 1;
        }
        getline(datosNP1,lineaLeida);
        getline(datosNP1,lineaLeida);
        if(lineaLeida == "1"){
            ds001 = 1;
            controlAuditado = 1;
        }
		getline(datosNP1,lineaLeida);
        getline(datosNP1,lineaLeida);
        if(lineaLeida == "1"){
            ct001 = 1;
            controlAuditado = 1;
        }
        datosNP1.close();
        // Imprimir controles auditados
        if(fp004)
            cout << "\033[32mOWISAM-FP-004: Deteccion de RogueAPs.\033[0m" << endl;
        if(di002)
            cout << "\033[32mOWISAM-DI-002: Descubrimiento de redes ocultas.\033[0m" << endl;
        if(au004)
            cout << "\033[32mOWISAM-AU-004: Captura de las claves transmitidas en el proceso de autenticación WPA.\033[0m" << endl;
        if(cp004)
            cout << "\033[32mOWISAM-CP-004: Detección de protocolos de cifrado no seguros.\033[0m" << endl;
        if(cf001)
            cout << "\033[32mOWISAM-CF-001: Identificación de redes con ESSID genérico.\033[0m" << endl;
        if(if003)
            cout << "\033[32mOWISAM-IF-003: Detección de reglas del Firewall mal configuradas.\033[0m" << endl;
        if(ds001)
            cout << "\033[32mOWISAM-DS-001: Detección de redes vulnerables ante ataques DoS.\033[0m" << endl;
		if(ct001)
            cout << "\033[32mOWISAM-CT-001: Captura de credenciales con RogueAP.\033[0m" << endl;
        //Imprimir controles aún no auditados
        cout << endl << "CONTROLES NO AUDITADOS" << endl;
        if(!fp004)
            cout << "[1] \033[31mOWISAM-FP-004: Deteccion de RogueAPs.\033[0m" << endl;
        if(!di002)
            cout << "[2] \033[31mOWISAM-DI-002: Descubrimiento de redes ocultas.\033[0m" << endl;
        if(!au004)
            cout << "[3] \033[31mOWISAM-AU-004: Captura de las claves transmitidas en el proceso de autenticación WPA.\033[0m" << endl;
        if(!cp004)
            cout << "[4] \033[31mOWISAM-CP-004: Detección de protocolos de cifrado no seguros.\033[0m" << endl;
        if(!cf001)
            cout << "[5] \033[31mOWISAM-CF-001: Identificación de redes con ESSID genérico.\033[0m" << endl;
        if(!if003)
            cout << "[6] \033[31mOWISAM-IF-003: Detección de reglas del Firewall mal configuradas.\033[0m" << endl;
        if(!ds001)
            cout << "[7] \033[31mOWISAM-DS-001: Detección de redes vulnerables ante ataques DoS.\033[0m" << endl;
		if(!ct001)
            cout << "[8] \033[31mOWISAM-CT-001: Captura de credenciales con RogueAP.\033[0m" << endl;
        if(!controlAuditado){
            cout << endl << "No hay controles auditados." << endl;
        }
        bool finPregunta = 0;
        while (!finPregunta){
            cout << "¿Que deseas hacer?" << endl << endl;
            cout << "      [1] Realizar un control." << endl;
            cout << "      [2] Exportar informe de auditoria." << endl;
            cout << "      [3] Cerrar proyecto." << endl << endl;
            while (!(cin >> respuesta)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Entrada inválida. Introduzca un número: ";
                }
            switch (respuesta){
                case 1:
                    finPregunta = 1;
                    cout << "¿Que control deseas realizar?";
                    int respuesta2;
                    while (!(cin >> respuesta2)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Entrada inválida. Introduzca un número: ";
                    }
                    switch (respuesta2){
                        case 1:
                            if(fp004)
                                cout << "El control OWISAM-FP-004 ya ha sido auditado." << endl;
                            else{
                            OWISAM_FP_004(carpetaProyecto);
                            fp004 = 1;
                            // Línea que queremos sobreescribir
                            int numeroDeLinea = 14;
                            // Declarar el arreglo de strings para almacenar las líneas
                            const int n = 10000; // Reemplaza con el tamaño adecuado
                            string lineasArchivo[n];
                            // Abrir el archivo en modo lectura
                            ifstream archivoLectura(rutaDatosNP1);
                            // Leer todas las líneas del archivo y almacenarlas en el arreglo
                            int indice = 0;
                            while (getline(archivoLectura, linea) && indice < n) {
                                lineasArchivo[indice] = linea;
                                indice++;
                            }
                            // Cerrar el archivo de lectura
                            archivoLectura.close();
                            // Sobreescribir la línea 14 con "1"
                            lineasArchivo[numeroDeLinea - 1] = "1";
                            // Abrir el archivo en modo escritura (esto sobreescribirá el archivo original)
                            ofstream archivoEscritura(rutaDatosNP1);
                            // Escribir las líneas modificadas de vuelta al archivo
                            for (int i = 0; i < indice; i++) {
                                archivoEscritura << lineasArchivo[i] << endl;
                            }
                            // Cerrar el archivo de escritura
                            archivoEscritura.close();
                            }
                        break;
                        case 2:
                            if(di002)
                                cout << "El control OWISAM-DI-002 ya ha sido auditado." << endl;
                            else{
                            OWISAM_DI_002(carpetaProyecto);
                            di002 = 1;
                            // Línea que queremos sobreescribir
                            int numeroDeLinea = 16;
                            // Declarar el arreglo de strings para almacenar las líneas
                            const int n = 10000; // Reemplaza con el tamaño adecuado
                            string lineasArchivo2[n];
                            // Abrir el archivo en modo lectura
                            ifstream archivoLectura2(rutaDatosNP1);
                            // Leer todas las líneas del archivo y almacenarlas en el arreglo
                            int indice = 0;
                            while (getline(archivoLectura2, linea) && indice < n) {
                                lineasArchivo2[indice] = linea;
                                indice++;
                            }
                            // Cerrar el archivo de lectura
                            archivoLectura2.close();
                            // Sobreescribir la línea 14 con "1"
                            lineasArchivo2[numeroDeLinea - 1] = "1";
                            // Abrir el archivo en modo escritura (esto sobreescribirá el archivo original)
                            ofstream archivoEscritura2(rutaDatosNP1);
                            // Escribir las líneas modificadas de vuelta al archivo
                            for (int i = 0; i < indice; i++) {
                                archivoEscritura2 << lineasArchivo2[i] << endl;
                            }
                            // Cerrar el archivo de escritura
                            archivoEscritura2.close();
                            }
                        break;
                        case 3:
                            if(au004)
                                cout << "El control OWISAM-AU-004 ya ha sido auditado." << endl;
                            else{
                            OWISAM_AU_004(carpetaProyecto);
                            au004 = 1;
                            // Línea que queremos sobreescribir
                            int numeroDeLinea = 18;
                            // Declarar el arreglo de strings para almacenar las líneas
                            const int n = 10000; // Reemplaza con el tamaño adecuado
                            string lineasArchivo3[n];
                            // Abrir el archivo en modo lectura
                            ifstream archivoLectura3(rutaDatosNP1);
                            // Leer todas las líneas del archivo y almacenarlas en el arreglo
                            int indice = 0;
                            while (getline(archivoLectura3, linea) && indice < n) {
                                lineasArchivo3[indice] = linea;
                                indice++;
                            }
                            // Cerrar el archivo de lectura
                            archivoLectura3.close();
                            // Sobreescribir la línea 14 con "1"
                            lineasArchivo3[numeroDeLinea - 1] = "1";
                            // Abrir el archivo en modo escritura (esto sobreescribirá el archivo original)
                            ofstream archivoEscritura3(rutaDatosNP1);
                            // Escribir las líneas modificadas de vuelta al archivo
                            for (int i = 0; i < indice; i++) {
                                archivoEscritura3 << lineasArchivo3[i] << endl;
                            }
                            // Cerrar el archivo de escritura
                            archivoEscritura3.close();
                            }
                        break;
                        case 4:
                            if(cp004)
                                cout << "El control OWISAM-CP-004 ya ha sido auditado." << endl;
                            else{
                            OWISAM_CP_004(carpetaProyecto);
                            cp004 = 1;
                            // Línea que queremos sobreescribir
                            int numeroDeLinea = 20;
                            // Declarar el arreglo de strings para almacenar las líneas
                            const int n = 10000; // Reemplaza con el tamaño adecuado
                            string lineasArchivo3[n];
                            // Abrir el archivo en modo lectura
                            ifstream archivoLectura3(rutaDatosNP1);
                            // Leer todas las líneas del archivo y almacenarlas en el arreglo
                            int indice = 0;
                            while (getline(archivoLectura3, linea) && indice < n) {
                                lineasArchivo3[indice] = linea;
                                indice++;
                            }
                            // Cerrar el archivo de lectura
                            archivoLectura3.close();
                            // Sobreescribir la línea 14 con "1"
                            lineasArchivo3[numeroDeLinea - 1] = "1";
                            // Abrir el archivo en modo escritura (esto sobreescribirá el archivo original)
                            ofstream archivoEscritura3(rutaDatosNP1);
                            // Escribir las líneas modificadas de vuelta al archivo
                            for (int i = 0; i < indice; i++) {
                                archivoEscritura3 << lineasArchivo3[i] << endl;
                            }
                            // Cerrar el archivo de escritura
                            archivoEscritura3.close();
                            }
                        break;
                        case 5:
                            if(cf001)
                                cout << "El control OWISAM-CF-001 ya ha sido auditado." << endl;
                            else{
                            OWISAM_CF_001(carpetaProyecto);
                            cf001 = 1;
                            // Línea que queremos sobreescribir
                            int numeroDeLinea = 22;
                            // Declarar el arreglo de strings para almacenar las líneas
                            const int n = 10000; // Reemplaza con el tamaño adecuado
                            string lineasArchivo3[n];
                            // Abrir el archivo en modo lectura
                            ifstream archivoLectura3(rutaDatosNP1);
                            // Leer todas las líneas del archivo y almacenarlas en el arreglo
                            int indice = 0;
                            while (getline(archivoLectura3, linea) && indice < n) {
                                lineasArchivo3[indice] = linea;
                                indice++;
                            }
                            // Cerrar el archivo de lectura
                            archivoLectura3.close();
                            // Sobreescribir la línea 14 con "1"
                            lineasArchivo3[numeroDeLinea - 1] = "1";
                            // Abrir el archivo en modo escritura (esto sobreescribirá el archivo original)
                            ofstream archivoEscritura3(rutaDatosNP1);
                            // Escribir las líneas modificadas de vuelta al archivo
                            for (int i = 0; i < indice; i++) {
                                archivoEscritura3 << lineasArchivo3[i] << endl;
                            }
                            // Cerrar el archivo de escritura
                            archivoEscritura3.close();
                            }
                        break;
                        case 6:
                            if(if003)
                                cout << "El control OWISAM-IF-003 ya ha sido auditado." << endl;
                            else{
                            OWISAM_IF_003(carpetaProyecto);
                            if003 = 1;
                            // Línea que queremos sobreescribir
                            int numeroDeLinea = 24;
                            // Declarar el arreglo de strings para almacenar las líneas
                            const int n = 10000; // Reemplaza con el tamaño adecuado
                            string lineasArchivo3[n];
                            // Abrir el archivo en modo lectura
                            ifstream archivoLectura3(rutaDatosNP1);
                            // Leer todas las líneas del archivo y almacenarlas en el arreglo
                            int indice = 0;
                            while (getline(archivoLectura3, linea) && indice < n) {
                                lineasArchivo3[indice] = linea;
                                indice++;
                            }
                            // Cerrar el archivo de lectura
                            archivoLectura3.close();
                            // Sobreescribir la línea 14 con "1"
                            lineasArchivo3[numeroDeLinea - 1] = "1";
                            // Abrir el archivo en modo escritura (esto sobreescribirá el archivo original)
                            ofstream archivoEscritura3(rutaDatosNP1);
                            // Escribir las líneas modificadas de vuelta al archivo
                            for (int i = 0; i < indice; i++) {
                                archivoEscritura3 << lineasArchivo3[i] << endl;
                            }
                            // Cerrar el archivo de escritura
                            archivoEscritura3.close();
                            }
                        break;
                        case 7:
                            if(ds001)
                                cout << "El control OWISAM-DS-001 ya ha sido auditado." << endl;
                            else{
                            OWISAM_DS_001(carpetaProyecto);
                            ds001 = 1;
                            // Línea que queremos sobreescribir
                            int numeroDeLinea = 26;
                            // Declarar el arreglo de strings para almacenar las líneas
                            const int n = 10000; // Reemplaza con el tamaño adecuado
                            string lineasArchivo3[n];
                            // Abrir el archivo en modo lectura
                            ifstream archivoLectura3(rutaDatosNP1);
                            // Leer todas las líneas del archivo y almacenarlas en el arreglo
                            int indice = 0;
                            while (getline(archivoLectura3, linea) && indice < n) {
                                lineasArchivo3[indice] = linea;
                                indice++;
                            }
                            // Cerrar el archivo de lectura
                            archivoLectura3.close();
                            // Sobreescribir la línea 14 con "1"
                            lineasArchivo3[numeroDeLinea - 1] = "1";
                            // Abrir el archivo en modo escritura (esto sobreescribirá el archivo original)
                            ofstream archivoEscritura3(rutaDatosNP1);
                            // Escribir las líneas modificadas de vuelta al archivo
                            for (int i = 0; i < indice; i++) {
                                archivoEscritura3 << lineasArchivo3[i] << endl;
                            }
                            // Cerrar el archivo de escritura
                            archivoEscritura3.close();
                            }
                        break;
						case 8:
                            if(ct001)
                                cout << "El control OWISAM-CT-001 ya ha sido auditado." << endl;
                            else{
                            OWISAM_CT_001(carpetaProyecto);
                            ct001 = 1;
                            // Línea que queremos sobreescribir
                            int numeroDeLinea = 28;
                            // Declarar el arreglo de strings para almacenar las líneas
                            const int n = 10000; // Reemplaza con el tamaño adecuado
                            string lineasArchivo3[n];
                            // Abrir el archivo en modo lectura
                            ifstream archivoLectura3(rutaDatosNP1);
                            // Leer todas las líneas del archivo y almacenarlas en el arreglo
                            int indice = 0;
                            while (getline(archivoLectura3, linea) && indice < n) {
                                lineasArchivo3[indice] = linea;
                                indice++;
                            }
                            // Cerrar el archivo de lectura
                            archivoLectura3.close();
                            // Sobreescribir la línea 14 con "1"
                            lineasArchivo3[numeroDeLinea - 1] = "1";
                            // Abrir el archivo en modo escritura (esto sobreescribirá el archivo original)
                            ofstream archivoEscritura3(rutaDatosNP1);
                            // Escribir las líneas modificadas de vuelta al archivo
                            for (int i = 0; i < indice; i++) {
                                archivoEscritura3 << lineasArchivo3[i] << endl;
                            }
                            // Cerrar el archivo de escritura
                            archivoEscritura3.close();
                            }
                        break;
                    }
                break;

                case 2:
                    finPregunta = 1;
                    cout << "Exportando informe." << endl;
                    sleep(3);
                    GenerarInforme(carpetaProyecto);
                break;

                case 3:
                    finPregunta = 1;
                    fin = 1;
                break;
                default:
                    cout << "Introduce una opción válida." << endl;
                break;

            }
        }
    }
}

void NuevoProyecto1(){
    system("clear");
    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool\033[0m" << endl;
    cout << "[NUEVO PROYECTO]"<< endl << endl;
    string nombreProyecto,nombreAuditor,empresaAuditada,rutaProyecto,empresaAuditora;
    int respuesta,respuesta1;
    bool datosCorrectos=0;
    cout << "Introduce la ruta donde guardar el proyecto:";
    cin >> rutaProyecto;
    cout << endl << "Introduce un nombre para el proyecto: [sin espacios]";
    cin >> nombreProyecto;
    cout << endl << "Introduce el nombre del auditor:";
    getline(cin,nombreAuditor);
    while(nombreAuditor.empty())
        getline(cin,nombreAuditor);
    cout << endl << "Introduce el nombre de la empresa auditora:";
    getline(cin,empresaAuditora);
    while(empresaAuditora.empty())
       getline(cin,empresaAuditora);
    cout << endl << "Introduce el nombre de la empresa auditada:";
    getline(cin,empresaAuditada);
    while(empresaAuditada.empty())
        getline(cin,empresaAuditada);
    while(datosCorrectos != 1){
        cout << endl << "Ruta del proyecto:" << "\033[33m" << rutaProyecto << "\033[0m" << endl;
        cout << "Nombre del proyecto:" << "\033[33m" << nombreProyecto << "\033[0m" << endl;
        cout << "Auditor:" << "\033[33m" << nombreAuditor << "\033[0m" << endl;
        cout << "Empresa auditora:" << "\033[33m" << empresaAuditora << "\033[0m" << endl;
        cout << "Empresa auditada" << "\033[33m" << empresaAuditada << "\033[0m" << endl;
        cout << "\033[33m¿Son estos datos correctos?\033[0m" << endl;
        cout << "[1]Sí" << endl;
        cout << "[2]No" << endl;
        while (!(cin >> respuesta)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Introduzca un número: ";
        }
        if (respuesta==1)
            datosCorrectos = 1;
        if (respuesta==2){
            cout << "\033[33m¿Que datos quieres cambiar?\033[0m" << endl;
            cout << "[1] Ruta del proyecto" << endl;
            cout << "[2] Nombre del proyecto" << endl;
            cout << "[3] Auditor" << endl;
            cout << "[4] Empresa auditora" << endl;
            cout << "[5] Empresa auditada" << endl;
            while (!(cin >> respuesta1)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada inválida. Introduzca un número: ";
                }
            switch (respuesta1){
                case 1:
                    cout << "Introduce la ruta donde guardar el proyecto:";
                    cin >> rutaProyecto;
                break;
                case 2:
                    cout << endl << "Introduce un nombre para el proyecto:";
                    cin >> nombreProyecto;
                break;
                case 3:
                    cout << endl << "Introduce el nombre del auditor:";
                    getline(cin,nombreAuditor);
                    while(nombreAuditor.empty())
                        getline(cin,nombreAuditor);
                break;
                case 4:
                   cout << endl << "Introduce el nombre de la empresa auditora:";
                   getline(cin,empresaAuditora);
                    while(empresaAuditora.empty())
                        getline(cin,empresaAuditora);
                break;
                case 5:
                    cout << endl << "Introduce el nombre de la empresa auditada:";
                    getline(cin,empresaAuditada);
                    while(empresaAuditada.empty())
                        getline(cin,empresaAuditada);
                break;
            }
        }

    }
    string rutaDatos,carpetaProyecto,comando;
    carpetaProyecto = rutaProyecto + "/" + nombreProyecto;
    cout << "Creando carpeta" << carpetaProyecto << endl;
    comando = "sudo mkdir -m 777 " + carpetaProyecto;
    system(comando.c_str());
    cout << "Creando fichero " << carpetaProyecto << "/" << nombreProyecto << ".jgm" << endl;
    system(comando.c_str());
    comando = "touch " + carpetaProyecto + "/datosNP1.jgm";
    system("clear");
    rutaDatos = carpetaProyecto + "/datosNP1.jgm";
    ofstream ficheroSalida(rutaDatos);
    if (!ficheroSalida.is_open()) {
        cerr << "No se pudo abrir el archivo para escritura." << endl;
    }
    ficheroSalida << "rutaProyecto" << endl << "carpetaProyecto" << endl << "nombreProyecto" << endl << "nombreAuditor" << endl << "empresaAuditora" << endl << "empresaAuditada" << endl;
    ficheroSalida << rutaProyecto << endl << carpetaProyecto << endl << nombreProyecto << endl << nombreAuditor << endl << empresaAuditora << endl << empresaAuditada << endl;
    ficheroSalida << "OWISAM-FP-004" << endl << 0 << endl;
    ficheroSalida << "OWISAM-DI-002" << endl << 0 << endl;
    ficheroSalida << "OWISAM-AU-004" << endl << 0 << endl;
    ficheroSalida << "OWISAM-CP-004" << endl << 0 << endl;
    ficheroSalida << "OWISAM-CF-001" << endl << 0 << endl;
    ficheroSalida << "OWISAM-IF-003" << endl << 0 << endl;
    ficheroSalida << "OWISAM-DS-001" << endl << 0 << endl;
	ficheroSalida << "OWISAM-CT-001" << endl << 0 << endl;
    ficheroSalida.close();
    PantallaAuditoria(carpetaProyecto);
}

void MenuPrincipal(){
    system("clear");
    bool fin = 0;
    cout << "\033[31mOWISAM AAHT - Automatic Auditoring Helper Tool v1.0\033[0m" << endl << endl;
    cout << "[INICIO]" << endl << endl;
    cout << "[1] Crear nuevo proyecto." << endl;
    cout << "[2] Cargar proyecto." << endl;
    cout << "[3] Opciones" << endl;
    cout << "[4] Utilidades de apoyo para realizar controles OWISAM-CT" << endl;
    cout << "[5] Salir" << endl;
    cout << endl << "\033[34mNota: la interfaz inalámbrica en la que se va a realizar la auditoría es \033[0m" << interfazCaptura << "\033[34m, la cual debe ser una interfaz válida con modo monitor activado. Si deseas configurar otra y activar el modo monitor, accede a Opciones/Configuracion de red.\033[0m" << endl;
    int input;
    // Validar entrada usuario
        while (!(cin >> input)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada inválida. Introduzca un número: ";
        }
   switch (input){
        case 1:
            NuevoProyecto1();
        break;
        case 2:

            while(!fin){
                cout << "Introduce la ruta del proyecto:";
                string carpetaProyecto;
                cin >> carpetaProyecto;
                cout << endl << "El proyecto se ubica en la ruta: " << carpetaProyecto << ", ¿es correcto?" << endl;
                cout << "[1] Si" << endl;
                cout << "[2] No" << endl;
                int respuesta;


                while (!(cin >> respuesta)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Entrada inválida. Introduzca un número: ";
                }
                if (respuesta == 1){
                    PantallaAuditoria(carpetaProyecto);
                    fin = 1;
                }
            }
        break;
        case 3:
            MenuOpciones();
        break;
        case 4:
            OWISAM_CT();
        break;
        case 5:
            exit(0);
        break;
        default:
            cout << "Opción no válida" << endl;
    }

}

int main()
{
    while(1){
    PantallaInicio();
    MenuPrincipal();
    }
    return 0;
}
