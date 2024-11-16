#include <Windows.h>  // Para los colores
#include <unistd.h>   // <-- Para sleep
#include <chrono>
#include <cstdlib>  // <-- Para rand
#include <ctime>    // <-- Para el srand
#include <iostream>
#include <string>

// Linea 229 para mostrar el numero ganador en cada ronda

const int JugadoresMax = 6;
const int kMax_numeros = 6;

struct Cant_numeros {
  int num[kMax_numeros];
};

struct Jugadores {
  std::string nombre_jugador;
  double monedero;
  int JugadoresActuales = 0;
  Cant_numeros cant_num;
  double MontoApostado;
  double ganancias;
  bool gano = false;
  std::string TipoApuesta[9];
  double TipoProbabilidad[9];
};

// Colores para los jugadores
const char *colores[] = {
    "\033[31m",  // Rojo
    "\033[32m",  // Verde
    "\033[33m",  // Amarillo (Dorado)
    "\033[34m",  // Azul
    "\033[35m",  // Magenta
    "\033[36m"   // Cian
};

// Declaracion de funciones
void MenuPrincipal();
void Tablero();
void RegistrarJugadores(Jugadores *jugadores[], int &cant_jugadores);
void JugadoresRegistrados(Jugadores *jugadores[], int &cant_jugadores);
void Instrucciones();
void Creditos();

// Declaraciones para el juego
void MenuApuestas(Jugadores *jugadores[], int &cant_jugadores);
void Pleno(Jugadores *jugadores[], int TurnoJugador, int &NumGanador);
void Dividida(Jugadores *jugadores[], int TurnoJugador, int &NumGanador);
void Calle(Jugadores *jugadores[], int TurnoJugador, int &NumGanador);
void Esquina(Jugadores *jugadores[], int TurnoJugador, int &NumGanador);
void Linea(Jugadores *jugadores[], int TurnoJugador, int &NumGanador);
void RojoNegro(Jugadores *jugadores[], int TurnoJugador, int &NumGanador);
void ApuestaParoImpar(Jugadores *jugadores[], int TurnoJugador,
                      int &NumGanador);
void Mitades(Jugadores *jugadores[], int TurnoJugador, int &NumGanador);
void ApuestaDocenas(Jugadores *jugadores[], int TurnoJugador, int &NumGanador);

// Declaraciones extras
void GenerarRandom(int &NumGanador);
std::string DeterminarColor(int numero);
void MostrarResultados(Jugadores *jugadores[], int cant_jugadores);
void GirarRuleta(int &NumGanador);
void Suspenso();

int main(int argc, char *argv[]) {
  srand(static_cast<unsigned>(time(0)));
  int opcion_menu, cant_jugadores, turno_jugador;
  Jugadores *jugadores[JugadoresMax];
  do {
    const std::string magenta = "\033[1;35m";  // Texto magenta
    const std::string reset = "\033[0m";       // Restablecer color
    const std::string cian = "\033[1;36m";     // Texto cian

    std::cout << magenta << "\n\n==========> RULETA UCA <==========\n" << reset;
    std::cout << cian << "Creado por: Discrete masters\n\n";

    MenuPrincipal();
    std::cout << "-> Digite su opcion: ";
    std::cin >> opcion_menu;

    switch (opcion_menu) {
      case 1: {
        RegistrarJugadores(jugadores, cant_jugadores);
        JugadoresRegistrados(jugadores, cant_jugadores);
        MenuApuestas(jugadores, cant_jugadores);
        break;
      }
      case 2:
        Instrucciones();
        break;

      case 3:
        Creditos();
        break;

      case 4:
        std::cout << std::endl << "Hasta la vista...";
        break;

      default:
        std::cout << std::endl
                  << "Opcion no valida. Digite de nuevo su opcion.";
        break;
    }
  } while (opcion_menu != 4);

  return 0;
}

void MenuPrincipal() {
  std::cout << "\033[33m\033[1m > 1.....Iniciar juego \033[0m\n";
  std::cout << "\033[32m\033[1m > 2.....Instrucciones \033[0m\n";
  std::cout << "\033[34m\033[1m > 3.....Creditos \033[0m\n";
  std::cout << "\033[31m\033[1m > 4.....Salir \033[0m\n";
}

void RegistrarJugadores(Jugadores *jugadores[], int &cant_jugadores) {
  do {
    std::cout << "\nIngrese la cantidad de jugadores que desean jugar (maximo "
              << JugadoresMax << "): ";
    std::cin >> cant_jugadores;
    std::cin.ignore();

    if (cant_jugadores < 1 || cant_jugadores > JugadoresMax) {
      std::cout << "Solo se pueden registrar de 1 a " << JugadoresMax
                << " jugadores. Intente de nuevo.\n";
    }
  } while (cant_jugadores < 1 || cant_jugadores > JugadoresMax);

  std::cout << '\n';

  for (int i = 0; i < cant_jugadores; i++) {
    jugadores[i] = new Jugadores;
    std::cout << "Jugador " << i + 1 << "\n";

    std::cout << "Nombre del jugador: ";
    std::getline(std::cin, jugadores[i]->nombre_jugador);

    std::cout << "Monedero inicial (Mayor a $5.00 y menor a $20000): $";
    std::cin >> jugadores[i]->monedero;

    while (jugadores[i]->monedero <= 5 || jugadores[i]->monedero >= 20000) {
      std::cout << "Cantidad no valida, ingrese una nueva: ";
      std::cin >> jugadores[i]->monedero;
    }
    std::cin.ignore();
    std::cout << "\n";
    jugadores[i]->JugadoresActuales++;
  }
}

void JugadoresRegistrados(Jugadores *jugadores[], int &cant_jugadores) {
  std::cout << '\n';
  std::cout << "Lista de jugadores registrados:\n\n";

  for (int i = 0; i < cant_jugadores; i++) {
    const char *color = colores[i % 6];
    std::cout << color;  // Aplicar el color
    std::cout << jugadores[i]->nombre_jugador << " saldo: $"
              << jugadores[i]->monedero << "\n"
              << "\033[0m\n";
    ;
  }
  std::cout << '\n';
}

void Instrucciones() {
  int volver_menu;
  std::cout << "\n\n\033[32m\033[1m--------------------------------------------"
               "-----------------------INSTRUCCIONES---------------------------"
               "-----------------------------------------\033[0m\n\n";
  std::cout << "\033[34m\033[1mTIPOS DE APUESTAS\033[0m\n\n";
  std::cout << "\033[31m\033[1mINTERNAS:\033[0m\n\n";
  std::cout
      << "\033[36m\033[1m* PLENO:\033[0m\n  Consiste en apostar a un "
         "numero directamente. Es la que tiene mayor riesgo y ofrece "
         "mejores pagos. Con una robabilidad de ganar de: 1/36 (2.7778%)\n\n";
  std::cout
      << "\033[36m\033[1m* DIVIDIDA:\033[0m\n  Consiste en una apuesta de dos "
         "numeros adyacentes, puede ser horizontal o verticalmente. Con una "
         "probabilidad de ganar de: 2/36 (5.55556%)\n\n";
  std::cout << "\033[36m\033[1m* CALLE:\033[0m\n  Consiste en una apuesta a "
               "una fila de tres numeros. Con una robabilidad de ganar de: "
               "3/36 (8.33333%)\n\n";
  std::cout
      << "\033[36m\033[1m* ESQUINA:\033[0m\n  Apuesta a cuatro numeros "
         "en un cuadrado. Con una robabilidad de ganar de: 4/36 (11.1111%)\n\n";
  std::cout << "\033[36m\033[1m* LINEA:\033[0m\n  Apuesta a seis numeros (dos "
               "filas adyacentes de tres). Con una robabilidad de ganar de: "
               "6/36 (16.6667%)\n\n";
  std::cout << "\033[31m\033[1mEXTERNAS:\033[0m\n\n";
  std::cout << "\033[36m\033[1m* ROJO O NEGRO:\033[0m\n  Apuesta a que el "
               "numero sea de un color especifico. Con una robabilidad de "
               "ganar de: 18/36 (50%)\n\n";
  std::cout << "\033[36m\033[1m* PAR O IMPAR:\033[0m\n  Apuesta a que el "
               "numero sea par o impar. Con una robabilidad de ganar de: 18/36 "
               "(50%)\n\n";
  std::cout << "\033[36m\033[1m* MITAD:\033[0m\n  Apuesta a que el numero este "
               "en la primera mitad (1-18) o segunda mitad (19-36). Con una "
               "robabilidad de ganar de: 18/36 (50%)\n\n";
  std::cout
      << "\033[36m\033[1m* DOCENAS:\033[0m\n  Apuesta a que el numero este en "
         "uno de los tres grupos de docenas (1-12, 13-24, o 25-36). Con una "
         "robabilidad de ganar de: 12/36 (33.3333%)\n\n";

  std::cout << "\033[36m\033[1m* EXTRAS:\033[0m\n  No colocar numeros cuando "
               "se le soliciten letras, "
               "ni colocar letras cuando se le soliciten numeros.\n  Seguir las "
               "indicaciones del programa para un desarrollo optimo. \n\n";

  std::cout << "\033[32m\033[1m------------------------------------------------"
               "---------------------------------------------------------------"
               "------------------------------------\033[0m\n\n";

  std::cout << "Presione 1 para volver al menu: ";
  std::cin >> volver_menu;

  while (volver_menu != 1) {
    std::cout << std::endl << "Numero no valido, ingrese otra vez el numero: ";
    std::cin >> volver_menu;
  }
}

void MenuApuestas(Jugadores *jugadores[], int &cant_jugadores) {
  int NumGanador = 0;
  int opcion = 0;
  static int Ronda = 1;

  // Definir colores

  GenerarRandom(NumGanador);
  // std::cout << "Numero ganador: " << NumGanador << "!\n";
  std::cout << '\n';

  const std::string magenta = "\033[1;35m";  // Texto magenta
  const std::string cian = "\033[1;36m";     // Texto cian
  const std::string reset = "\033[0m";       // Restablecer color
  std::cout << cian
            << "+--------------------------------------------------------------"
               "-------------------------+"
            << reset << std::endl;
  std::cout << cian << "|                                      Ronda: " << Ronda
            << "                                         |" << reset
            << std::endl;
  std::cout << cian
            << "+--------------------------------------------------------------"
               "-------------------------+"
            << reset << std::endl;

  for (int i = 0; i < cant_jugadores; i++) {
    // Verificar si el saldo del jugador es menor a $5
    if (jugadores[i]->monedero <= 5) {
      std::cout
          << "\nEl jugador " << jugadores[i]->nombre_jugador
          << " no tiene suficiente saldo para apostar, su saldo actual es de: $"
          << jugadores[i]->monedero << std::endl;
      continue;  // Saltar al siguiente jugador
    }
  }

  srand(static_cast<unsigned>(time(0)));
  bool EstadoApuesta = false;

  // Verificar si todos los jugadores tienen saldo insuficiente
  bool todosConSaldoSuficiente = false;
  for (int i = 0; i < cant_jugadores; i++) {
    if (jugadores[i]->monedero > 5) {
      todosConSaldoSuficiente = true;
      break;
    }
  }

  if (!todosConSaldoSuficiente) {
    std::cout << "\nTodos los jugadores tienen menos de $6 y no pueden "
                 "apostar.\n\n";
    std::cout << "Fin del juego.\n\n";
    return;
  }

  for (int i = 0; i < cant_jugadores; i++) {
    std::cout << "\033[1;36m\nTurno del jugador " << i + 1 << ": " << "\033[0m"
              << jugadores[i]->nombre_jugador << std::endl;

    if (jugadores[i]->monedero > 5) {
      Tablero();

      std ::cout << "----------------Apuestas y su probabilidad de "
                    "ganar----------------\n";
      std ::cout << "-Entre menor sea la probabilidad de ganar, mayor sera su "
                    "ganancia.\n";
      std ::cout << "-Entre mayor sea la probabilidad de ganar, menor sera su "
                    "ganancia.\n\n";
      std ::cout << "> 1... Pleno. P(1/36)\n";
      std ::cout << "> 2... Dividida. P(2/36)\n";
      std ::cout << "> 3... Calle. P(3/36)\n";
      std ::cout << "> 4... Esquina. P(4/36)\n";
      std ::cout << "> 5... Linea. P(6/36)\n";
      std ::cout << "> 6... Rojo o Negro. P(18/36)\n";
      std ::cout << "> 7... Par o impar. P(18/36)\n";
      std ::cout << "> 8... Mitades. P(18/36)\n";
      std ::cout << "> 9... Docenas. P(12/36)\n";
      std::cout << "\nSeleccione un tipo de apuesta: ";
      std::cin >> opcion;

      while (opcion < 1 || opcion > 9) {
        std::cout << "Opcion de apuesta no valida. Digite de nuevo la opcion: ";
        std::cin >> opcion;
      }

      switch (opcion) {
        case 1:
          Pleno(jugadores, i, NumGanador);
          break;

        case 2:
          Dividida(jugadores, i, NumGanador);
          break;

        case 3:
          Calle(jugadores, i, NumGanador);
          break;

        case 4:
          Esquina(jugadores, i, NumGanador);
          break;

        case 5:
          Linea(jugadores, i, NumGanador);
          break;

        case 6:
          RojoNegro(jugadores, i, NumGanador);
          break;

        case 7:
          ApuestaParoImpar(jugadores, i, NumGanador);
          break;

        case 8:
          Mitades(jugadores, i, NumGanador);
          break;

        case 9:
          ApuestaDocenas(jugadores, i, NumGanador);
          break;
      }
    } else {
      std::cout << std::endl
                << "No tiene suficiente saldo para apostar." << std::endl;
      std::cout << "Ya que su monedero es de: $" << jugadores[i]->monedero
                << std::endl;
    }
  }

  EstadoApuesta = true;

  if (EstadoApuesta == true) {
    GirarRuleta(NumGanador);
    MostrarResultados(jugadores, cant_jugadores);
  }

  // Preguntar si se quiere jugar otra ronda
  int continuar;
  std::cout << std::endl << "Desea jugar otra ronda? \n";
  std::cout << "1.Jugar otra ronda\n";
  std::cout << "2.Volver al menu principal\n";
  std::cout << "Digite su opcion: ";
  std::cin >> continuar;

  while (continuar < 1 || continuar > 2) {
    std::cout << "Numero no valido. Digite de nuevo el numero: ";
    std::cin >> continuar;
  }

  // Manejo de la decision del jugador
  if (continuar == 1) {
    Ronda++;
    MenuApuestas(jugadores, cant_jugadores);
  } else {
    std::cout << std::endl << "Gracias por jugar. Hasta la proxima.\n";
    return;
  }
}

void Pleno(Jugadores *jugadores[], int TurnoJugador, int &NumGanador) {
  int elegido;
  double probabilidadGanar = 1.0 / 36.0;
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Pleno";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  std::cout << std::endl
            << "Su saldo inicial es de: $" << jugadores[TurnoJugador]->monedero
            << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout << std::endl
            << "Digite el numero al que quiere apostar (Del 1 al 36): ";
  std::cin >> elegido;

  while (elegido > 36 || elegido < 1) {
    std::cout << "Numero no valido. Digite de nuevo el numero: ";
    std::cin >> elegido;
  }

  std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
  jugadores[TurnoJugador]->monedero -=
      jugadores[TurnoJugador]
          ->MontoApostado;  // Se resta del monedero la cantidad apostada

  std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100 << "%"
            << std::endl;

  if (elegido == NumGanador) {
    int ganancias = jugadores[TurnoJugador]->MontoApostado * 35;
    jugadores[TurnoJugador]->monedero +=
        ganancias + jugadores[TurnoJugador]->MontoApostado;
    jugadores[TurnoJugador]->ganancias =
        ganancias + jugadores[TurnoJugador]->MontoApostado;
    jugadores[TurnoJugador]->gano = true;
  } else {
    jugadores[TurnoJugador]->ganancias = 0;
    jugadores[TurnoJugador]->gano = false;
  }
}

void Dividida(Jugadores *jugadores[], int TurnoJugador, int &NumGanador) {
  bool apuesta = false;
  double probabilidadGanar = 2.0 / 36.0;
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Dividida";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  int adyacentes[57][2] = {
      {1, 2},   {1, 4},   {2, 3},   {2, 5},   {3, 6},   {4, 5},   {4, 7},
      {5, 6},   {5, 8},   {6, 9},   {7, 8},   {7, 10},  {8, 9},   {8, 11},
      {9, 12},  {10, 11}, {10, 13}, {11, 12}, {11, 14}, {12, 15}, {13, 14},
      {13, 16}, {14, 15}, {14, 17}, {15, 18}, {16, 17}, {16, 19}, {17, 18},
      {17, 20}, {18, 21}, {19, 20}, {19, 22}, {20, 21}, {20, 23}, {21, 24},
      {22, 23}, {22, 25}, {23, 24}, {23, 26}, {24, 27}, {25, 26}, {25, 28},
      {26, 27}, {26, 29}, {27, 30}, {28, 29}, {28, 31}, {29, 30}, {29, 32},
      {30, 33}, {31, 32}, {31, 34}, {32, 33}, {32, 35}, {33, 36}, {34, 35},
      {35, 36}};

  std::cout << std::endl
            << "Su saldo inicial es de: $" << jugadores[TurnoJugador]->monedero
            << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout << std::endl
            << "Escoja dos numeros adyacentes (juntos) a los cuales apostar.\n";

  Tablero();  // Se llama a la funcion tablero para que imprima la tabla de
              // juego

  std::cout << "Elija los numeros adyacentes que va a escoger segun la tabla: "
            << std::endl;

  do {
    for (int i = 0; i < 2; i++) {
      std::cout << "Numero " << i + 1 << ": ";
      std::cin >> jugadores[TurnoJugador]->cant_num.num[i];
    }

    for (int i = 0; i < 57; i++) {
      if (adyacentes[i][0] == jugadores[TurnoJugador]->cant_num.num[0] &&
              adyacentes[i][1] == jugadores[TurnoJugador]->cant_num.num[1] ||
          adyacentes[i][0] == jugadores[TurnoJugador]->cant_num.num[1] &&
              adyacentes[i][1] == jugadores[TurnoJugador]->cant_num.num[0]) {
        apuesta = true;
      }
    }

    if (apuesta) {
      std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
      jugadores[TurnoJugador]->monedero -=
          jugadores[TurnoJugador]
              ->MontoApostado;  // Se resta del monedero la cantidad apostada

      std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100
                << "%" << std::endl;

      if (NumGanador == jugadores[TurnoJugador]->cant_num.num[0] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[1]) {
        int ganancias = jugadores[TurnoJugador]->MontoApostado * 17;
        jugadores[TurnoJugador]->monedero +=
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->ganancias =
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->gano = true;
      } else {
        jugadores[TurnoJugador]->ganancias = 0;
      }
    } else {
      std::cout << std::endl
                << "La apuesta no se pudo realizar, intente de nuevo."
                << std::endl;
    }
  } while (apuesta == false);
}

void Calle(Jugadores *jugadores[], int TurnoJugador, int &NumGanador) {
  bool apuesta = false;
  double probabilidadGanar = 3.0 / 36.0;
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Calle";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  int calles[12][3] = {{1, 2, 3},    {4, 5, 6},    {7, 8, 9},    {10, 11, 12},
                       {13, 14, 15}, {16, 17, 18}, {19, 20, 21}, {22, 23, 24},
                       {25, 26, 27}, {28, 29, 30}, {31, 32, 33}, {34, 35, 36}};

  std::cout << std::endl
            << "Su saldo inicial es de: $" << jugadores[TurnoJugador]->monedero
            << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout << std::endl
            << "Filas a las que puede apostar (Del 1 al 3, 4 al 6, etc.).\n";
  Tablero();

  std::cout << "Elija los numeros de la fila que va a escoger segun la tabla: "
            << std::endl;

  do {
    for (int i = 0; i < 3; i++) {
      std::cout << "Numero " << i + 1 << ": ";
      std::cin >> jugadores[TurnoJugador]->cant_num.num[i];
    }

    for (int i = 0; i < 12; i++) {
      if (calles[i][0] == jugadores[TurnoJugador]->cant_num.num[0] &&
              calles[i][1] == jugadores[TurnoJugador]->cant_num.num[1] &&
              calles[i][2] == jugadores[TurnoJugador]->cant_num.num[2] ||
          calles[i][0] == jugadores[TurnoJugador]->cant_num.num[2] &&
              calles[i][1] == jugadores[TurnoJugador]->cant_num.num[0] &&
              calles[i][2] == jugadores[TurnoJugador]->cant_num.num[1] ||
          calles[i][0] == jugadores[TurnoJugador]->cant_num.num[1] &&
              calles[i][1] == jugadores[TurnoJugador]->cant_num.num[2] &&
              calles[i][2] == jugadores[TurnoJugador]->cant_num.num[0] ||
          calles[i][0] == jugadores[TurnoJugador]->cant_num.num[0] &&
              calles[i][1] == jugadores[TurnoJugador]->cant_num.num[2] &&
              calles[i][2] == jugadores[TurnoJugador]->cant_num.num[1] ||
          calles[i][0] == jugadores[TurnoJugador]->cant_num.num[1] &&
              calles[i][1] == jugadores[TurnoJugador]->cant_num.num[0] &&
              calles[i][2] == jugadores[TurnoJugador]->cant_num.num[2] ||
          calles[i][0] == jugadores[TurnoJugador]->cant_num.num[2] &&
              calles[i][1] == jugadores[TurnoJugador]->cant_num.num[1] &&
              calles[i][2] == jugadores[TurnoJugador]->cant_num.num[0]) {
        apuesta = true;
      }
    }

    if (apuesta) {
      std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
      jugadores[TurnoJugador]->monedero -=
          jugadores[TurnoJugador]
              ->MontoApostado;  // Se resta del monedero la cantidad apostada

      std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100
                << "%" << std::endl;

      if (NumGanador == jugadores[TurnoJugador]->cant_num.num[0] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[1] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[2]) {
        int ganancias = jugadores[TurnoJugador]->MontoApostado * 11;
        jugadores[TurnoJugador]->monedero +=
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->ganancias =
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->gano = true;
      } else {
        jugadores[TurnoJugador]->ganancias = 0;
        jugadores[TurnoJugador]->gano = false;
      }
    } else {
      std::cout << std::endl
                << "La apuesta no se pudo realizar, intente de nuevo."
                << std::endl;
    }
  } while (apuesta == false);
}

void Esquina(Jugadores *jugadores[], int TurnoJugador, int &NumGanador) {
  bool apuesta = false;
  double probabilidadGanar = 4.0 / 36.0;
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Esquina";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  int cuadrados[22][4] = {
      {1, 2, 4, 5},     {2, 3, 5, 6},     {4, 5, 7, 8},     {5, 6, 8, 9},
      {7, 8, 10, 11},   {8, 9, 11, 12},   {10, 11, 13, 14}, {11, 12, 14, 15},
      {13, 14, 16, 17}, {14, 15, 17, 18}, {16, 17, 19, 20}, {17, 18, 20, 21},
      {19, 20, 22, 23}, {20, 21, 23, 24}, {22, 23, 25, 26}, {23, 24, 26, 27},
      {25, 26, 28, 29}, {26, 27, 29, 30}, {28, 29, 31, 32}, {29, 30, 32, 33},
      {31, 32, 34, 35}, {32, 33, 35, 36}};

  std::cout << std::endl
            << "Su saldo inicial es de: $" << jugadores[TurnoJugador]->monedero
            << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout
      << std::endl
      << "Cuadrados de numeros a los que puede apostar (Ej: 1, 2, 4, 5).\n";
  Tablero();

  std::cout << "Elija los numeros que formen un cuadrado para apostar: "
            << std::endl;

  do {
    for (int i = 0; i < 4; i++) {
      std::cout << "Numero " << i + 1 << ": ";
      std::cin >> jugadores[TurnoJugador]->cant_num.num[i];
    }

    // Comprobar si los numeros del usuario coinciden con algún cuadrado
    for (int i = 0; i < 22; i++) {   // Para cada cuadrado
      int count = 0;                 // Contador de coincidencias
      for (int j = 0; j < 4; j++) {  // Para cada numero en el cuadrado
        for (int k = 0; k < 4;
             k++) {  // Para cada numero ingresado por el usuario
          if (cuadrados[i][j] == jugadores[TurnoJugador]->cant_num.num[k]) {
            count++;  // Se encontró una coincidencia
            break;    // Salir del bucle si se encuentra una coincidencia
          }
        }
      }

      // Si se encontraron 4 coincidencias, significa que todos los numeros
      // coinciden
      if (count == 4) {
        apuesta = true;  // Se encontró una coincidencia completa
        break;           // Salir del bucle si se encontró una coincidencia
      }
    }

    // Resultado
    if (apuesta) {
      std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
      jugadores[TurnoJugador]->monedero -=
          jugadores[TurnoJugador]
              ->MontoApostado;  // Se resta del monedero la cantidad apostada

      std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100
                << "%" << std::endl;

      if (NumGanador == jugadores[TurnoJugador]->cant_num.num[0] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[1] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[2] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[3]) {
        int ganancias = jugadores[TurnoJugador]->MontoApostado * 8;
        jugadores[TurnoJugador]->monedero +=
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->ganancias =
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->gano = true;
      } else {
        jugadores[TurnoJugador]->ganancias = 0;
        jugadores[TurnoJugador]->gano = false;
      }
    } else {
      std::cout << std::endl
                << "La apuesta no se pudo realizar, intente de nuevo."
                << std::endl;
    }
  } while (apuesta == false);
}

void Linea(Jugadores *jugadores[], int TurnoJugador, int &NumGanador) {
  bool apuesta = false;
  double probabilidadGanar = 6.0 / 36.0;
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Linea";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  int lineas[11][6] = {{1, 2, 3, 4, 5, 6},       {4, 5, 6, 7, 8, 9},
                       {7, 8, 9, 10, 11, 12},    {10, 11, 12, 13, 14, 15},
                       {13, 14, 15, 16, 17, 18}, {16, 17, 18, 19, 20, 21},
                       {19, 20, 21, 22, 23, 24}, {22, 23, 24, 25, 26, 27},
                       {25, 26, 27, 28, 29, 30}, {28, 29, 30, 31, 32, 33},
                       {31, 32, 33, 34, 35, 36}};

  std::cout << std::endl
            << "Su saldo inicial es de: $" << jugadores[TurnoJugador]->monedero
            << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout << std::endl << "Filas de numeros a los que puede apostar.\n";

  Tablero();
  std::cout << "Seleccione 6 numeros contiguos en fila: " << std::endl;

  do {
    for (int i = 0; i < 6; i++) {
      std::cout << "Numero " << i + 1 << ": ";
      std::cin >> jugadores[TurnoJugador]->cant_num.num[i];
    }

    for (int i = 0; i < 11; i++) {   // Para cada cuadrado
      int count = 0;                 // Contador de coincidencias
      for (int j = 0; j < 6; j++) {  // Para cada numero en el cuadrado
        for (int k = 0; k < 6;
             k++) {  // Para cada numero ingresado por el usuario
          if (lineas[i][j] == jugadores[TurnoJugador]->cant_num.num[k]) {
            count++;  // Se encontró una coincidencia
            break;    // Salir del bucle si se encuentra una coincidencia
          }
        }
      }

      // Si se encontraron 4 coincidencias, significa que todos los numeros
      // coinciden
      if (count == 6) {
        apuesta = true;  // Se encontró una coincidencia completa
        break;           // Salir del bucle si se encontró una coincidencia
      }
    }

    if (apuesta) {
      std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
      jugadores[TurnoJugador]->monedero -=
          jugadores[TurnoJugador]
              ->MontoApostado;  // Se resta del monedero la cantidad apostada

      std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100
                << "%" << std::endl;

      if (NumGanador == jugadores[TurnoJugador]->cant_num.num[0] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[1] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[2] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[3] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[4] ||
          NumGanador == jugadores[TurnoJugador]->cant_num.num[5]) {
        int ganancias = jugadores[TurnoJugador]->MontoApostado * 5;
        jugadores[TurnoJugador]->monedero +=
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->ganancias =
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->gano = true;
      } else {
        jugadores[TurnoJugador]->ganancias = 0;
        jugadores[TurnoJugador]->gano = false;
      }
    } else {
      std::cout << std::endl
                << "La apuesta no se pudo realizar, intente de nuevo."
                << std::endl;
    }

  } while (apuesta == false);
}

void RojoNegro(Jugadores *jugadores[], int TurnoJugador, int &NumGanador) {
  int color_apostado;
  double probabilidadGanar = 18.0 / 36.0;
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Rojo o Negro";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  std::cout << "\nSu saldo inicial es de: $"
            << jugadores[TurnoJugador]->monedero << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout << std::endl << "Elige un color (1 para negro o 2 para rojo): ";
  std::cin >> color_apostado;

  while (color_apostado < 1 || color_apostado > 2) {
    std::cout << "Opcion invalida. Digite de nuevo: ";
    std::cin >> color_apostado;
  }

  std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
  jugadores[TurnoJugador]->monedero -=
      jugadores[TurnoJugador]
          ->MontoApostado;  // Se resta del monedero la cantidad apostada

  std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100 << "%"
            << std::endl;

  std::string color_ganador = DeterminarColor(NumGanador);

  if ((color_apostado == 1 && color_ganador == "negro") ||
      (color_apostado == 2 && color_ganador == "rojo")) {
    int ganancias = jugadores[TurnoJugador]->MontoApostado * 1;
    jugadores[TurnoJugador]->monedero +=
        ganancias + jugadores[TurnoJugador]->MontoApostado;
    jugadores[TurnoJugador]->ganancias =
        ganancias + jugadores[TurnoJugador]->MontoApostado;
    jugadores[TurnoJugador]->gano = true;
  } else {
    jugadores[TurnoJugador]->ganancias = 0;
  }
}

void ApuestaParoImpar(Jugadores *jugadores[], int TurnoJugador,
                      int &NumGanador) {
  int opcion;
  double probabilidadGanar = 18.0 / 36.0;
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Par o impar";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  std::cout << "\nSu saldo inicial es de: $"
            << jugadores[TurnoJugador]->monedero << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout << std::endl
            << "Seleccione al tipo de numero que quiere apostar:\n";
  std::cout << "1.Par\n";
  std::cout << "2.Impar\n";
  std::cout << "Digite su opcion: ";
  std::cin >> opcion;

  while (opcion < 1 || opcion > 2) {
    std::cout << "Digite una opcion valida (1 para Par o 2 para Impar): ";
    std::cin >> opcion;
  }

  std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
  jugadores[TurnoJugador]->monedero -=
      jugadores[TurnoJugador]
          ->MontoApostado;  // Se resta del monedero la cantidad apostada

  std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100
            << "%\n";

  bool gano = (opcion == 1 && NumGanador % 2 == 0) ||
              (opcion == 2 && NumGanador % 2 != 0);

  if (gano) {
    int ganancias = jugadores[TurnoJugador]->MontoApostado * 1;
    jugadores[TurnoJugador]->monedero +=
        ganancias + jugadores[TurnoJugador]->MontoApostado;
    jugadores[TurnoJugador]->ganancias =
        ganancias + jugadores[TurnoJugador]->MontoApostado;
    jugadores[TurnoJugador]->gano = true;
  } else {
    jugadores[TurnoJugador]->ganancias = 0;
    jugadores[TurnoJugador]->gano = false;
  }
}

void Mitades(Jugadores *jugadores[], int TurnoJugador, int &NumGanador) {
  int opcion;
  bool apuesta = false;
  double probabilidadGanar = 18.0 / 36.0;
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Mitades";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  std::cout << "\nSu saldo inicial es de: $"
            << jugadores[TurnoJugador]->monedero << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout << std::endl << "Seleccione la mitad a la que quiere apostar:\n";
  std::cout << "1. (1-18)\n";
  std::cout << "2. (19-36)\n";
  std::cout << "Digite su opcion: ";
  std::cin >> opcion;

  while (opcion < 1 || opcion > 2) {
    std::cout << "Opcion no valida, intente de nuevo: ";
    std::cin >> opcion;
  }

  std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
  jugadores[TurnoJugador]->monedero -=
      jugadores[TurnoJugador]
          ->MontoApostado;  // Se resta del monedero la cantidad apostada

  std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100 << "%"
            << std::endl;

  switch (opcion) {
    case 1:
      for (int i = 1; i <= 18; i++) {
        if (NumGanador == i) {
          apuesta = true;
        }
      }

      if (apuesta) {
        int ganancias = jugadores[TurnoJugador]->MontoApostado * 1;
        jugadores[TurnoJugador]->monedero +=
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->ganancias =
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->gano = true;
      } else {
        jugadores[TurnoJugador]->ganancias = 0;
      }
      break;

    case 2:
      for (int i = 19; i <= 36; i++) {
        if (NumGanador == i) {
          apuesta = true;
        }
      }

      if (apuesta) {
        int ganancias = jugadores[TurnoJugador]->MontoApostado * 1;
        jugadores[TurnoJugador]->monedero +=
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->ganancias =
            ganancias + jugadores[TurnoJugador]->MontoApostado;
        jugadores[TurnoJugador]->gano = true;
      } else {
        jugadores[TurnoJugador]->ganancias = 0;
      }

    default:
      break;
  }
}

void ApuestaDocenas(Jugadores *jugadores[], int TurnoJugador, int &NumGanador) {
  int docenaElegida;
  double probabilidadGanar =
      12.0 / 36.0;  // La probabilidad de ganar en una apuesta a docenas.
  jugadores[TurnoJugador]->TipoApuesta[TurnoJugador] = "Docenas";
  jugadores[TurnoJugador]->TipoProbabilidad[TurnoJugador] =
      probabilidadGanar * 100;

  std::cout << "\nSu saldo inicial es de: $"
            << jugadores[TurnoJugador]->monedero << std::endl;

  std::cout << "Digite la cantidad a apostar (Entre $5.00 y $5000.00): $";
  std::cin >> jugadores[TurnoJugador]->MontoApostado;

  while (jugadores[TurnoJugador]->MontoApostado >
             jugadores[TurnoJugador]->monedero ||
         (jugadores[TurnoJugador]->MontoApostado < 5 ||
          jugadores[TurnoJugador]->MontoApostado > 5000)) {
    std::cout << "Cantidad no valida. Digite de nuevo la cantidad a apostar: $";
    std::cin >> jugadores[TurnoJugador]->MontoApostado;
  }

  std::cout << std::endl << "Seleccione la docena a la que quiere apostar:\n";
  std::cout << "1. Primera docena (1 - 12)\n";
  std::cout << "2. Segunda docena (13 - 24)\n";
  std::cout << "3. Tercera docena (25 - 36)\n";
  std::cout << "Digite su opcion: ";
  std::cin >> docenaElegida;

  // Validar la opción de la docena.
  while (docenaElegida < 1 || docenaElegida > 3) {
    std::cout << "Digite una opcion valida (1, 2 o 3): ";
    std::cin >> docenaElegida;
  }

  std::cout << std::endl << "Apuesta realizada." << std::endl << std::endl;
  jugadores[TurnoJugador]->monedero -=
      jugadores[TurnoJugador]
          ->MontoApostado;  // Se resta del monedero la cantidad apostada

  std::cout << "La probabilidad de ganar es: " << probabilidadGanar * 100 << "%"
            << std::endl;

  bool gano = false;
  if ((docenaElegida == 1 && NumGanador >= 1 && NumGanador <= 12) ||
      (docenaElegida == 2 && NumGanador >= 13 && NumGanador <= 24) ||
      (docenaElegida == 3 && NumGanador >= 25 && NumGanador <= 36)) {
    gano = true;
  }

  if (gano) {
    double ganancias =
        jugadores[TurnoJugador]->MontoApostado *
        3;  // La apuesta a docenas paga 2 a 1, total 3 veces la apuesta
    jugadores[TurnoJugador]->monedero +=
        ganancias + jugadores[TurnoJugador]
                        ->MontoApostado;  // Actualiza el saldo del jugador.
    jugadores[TurnoJugador]->ganancias =
        ganancias +
        jugadores[TurnoJugador]->MontoApostado;  // Calcula las ganancias netas.
    jugadores[TurnoJugador]->gano = true;        // Marca que el jugador ganó.
  } else {
    jugadores[TurnoJugador]->ganancias = 0;  // Si pierde, las ganancias son 0.
    jugadores[TurnoJugador]->gano = false;   // Marca que el jugador no ganó.
  }
}

std::string DeterminarColor(int numero) {
  switch (numero) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
    case 12:
    case 14:
    case 16:
    case 18:
    case 19:
    case 21:
    case 23:
    case 25:
    case 27:
    case 30:
    case 32:
    case 34:
    case 36:
      return "rojo";
    case 2:
    case 4:
    case 6:
    case 8:
    case 10:
    case 11:
    case 13:
    case 15:
    case 17:
    case 20:
    case 22:
    case 24:
    case 26:
    case 28:
    case 29:
    case 31:
    case 33:
    case 35:
      return "negro";
    default:
      return "ninguno";
  }
}

void Creditos() {
  int volver_menu;

  const std::string magenta = "\033[1;35m";  // Texto magenta
  const std::string cian = "\033[1;36m";     // Texto cian
  const std::string reset = "\033[0m";       // Restablecer color
  std::cout << '\n';
  std::cout << '\n';
  std::cout << magenta << "+-----------------------------------------------+"
            << reset << std::endl;
  std::cout << magenta << "|                   Creditos                    |"
            << reset << std::endl;
  std::cout << magenta << "+-----------------------------------------------+"
            << reset << std::endl;

  std::cout << std::endl
            << cian << "             [ > DISCRETE MASTERS < ]\n"
            << reset << cian << std::endl;
  std::cout << cian << "Diego Antonio Retana Saravia      " << reset << cian
            << "----> #00229524\n"
            << reset;
  std::cout << cian << "Carlos Gerardo Santos Pocasangre  " << reset << cian
            << "----> #00087624\n"
            << reset;
  std::cout << cian << "David Alessandro Ventura Montoya  " << reset << cian
            << "----> #00089724\n"
            << reset;
  std::cout << cian << "Juan Carlos Rivera Melendez       " << reset << cian
            << "----> #00029324\n"
            << reset << std::endl;

  std::cout << magenta << "+-----------------------------------------------+"
            << reset << std::endl;
  std::cout << magenta << "|               Fin de los creditos             |"
            << reset << std::endl;
  std::cout << magenta << "+-----------------------------------------------+"
            << reset << std::endl;

  std::cout << std::endl << "Presione 1 para volver al menu: ";
  std::cin >> volver_menu;

  while (volver_menu != 1) {
    std::cout << std::endl << "Numero no valido, ingrese otra vez el numero: ";
    std::cin >> volver_menu;
  }
}

void GenerarRandom(int &NumGanador) {
  int random;

  random = rand() % 36;
  NumGanador = random + 1;
}

void GirarRuleta(int &NumGanador) {
  const int delay = 100;
  int vuelta;

  std::cout << "\nLa ruleta esta girando";
  Suspenso();

  std::cout << '\n' << '\n';

  for (int i = 0; i < 20; ++i) {
    vuelta = rand() % 36;
    std::cout << "Numero: " << vuelta << "\r";
    std::cout.flush();

    Sleep(delay);
  }
  std::cout << "Numero ganador: \033[1m\033[34m" << NumGanador << "\033[0m!\n";
}

void Suspenso()  // Para que aparezca el ganador en cierto tiempo
{
  int nEsp, i;
  for (nEsp = 1; nEsp <= 3; nEsp++) {
    for (i = 1; i < nEsp; i++) {
      std::cout << ". ";
      sleep(1);
    }
  }
}

void MostrarResultados(Jugadores *jugadores[], int cant_jugadores) {
  const std::string magenta = "\033[1;35m";  // Texto magenta
  const std::string cian = "\033[1;36m";     // Texto cian
  const std::string reset = "\033[0m";       // Restablecer color

  std::cout << '\n';
  std::cout << cian << "+-------------------------------------------+" << reset
            << std::endl;
  std::cout << cian << "|            Tabla de Resultados            |" << reset
            << std::endl;
  std::cout << cian << "+-------------------------------------------+" << reset
            << std::endl;

  for (int i = 0; i < cant_jugadores; i++) {
    if (jugadores[i] == nullptr) {
      break;
    }

    std::cout << cian << "| " << reset << magenta
              << "Resultados para el jugador " << i + 1 << " - "
              << jugadores[i]->nombre_jugador << ": " << reset << std::endl;

    if (jugadores[i]->gano) {
      std::cout << cian << "| " << reset << "Ganaste! Tu ganancia fue de "
                << "\033[1;32m" << "+$" << jugadores[i]->ganancias
                << "\033[0m\n";
      std::cout << cian << "| " << reset << "Nuevo saldo: $"
                << jugadores[i]->monedero << '\n';
      std::cout << cian << "| " << reset
                << "Tipo de apuesta: " << jugadores[i]->TipoApuesta[i] << '\n';
      std::cout << cian << "| " << reset << "Probabilidad de ganar: "
                << jugadores[i]->TipoProbabilidad[i] << "%\n";
    } else {
      double perdida = jugadores[i]->MontoApostado;
      std::cout << cian << "| " << reset << "Perdiste. Tu perdida fue de "
                << "\033[1;31m" << "-$" << perdida << "\033[0m\n";
      std::cout << cian << "| " << reset << "Saldo restante: $"
                << jugadores[i]->monedero << '\n';
      std::cout << cian << "| " << reset
                << "Tipo de apuesta: " << jugadores[i]->TipoApuesta[i] << '\n';
      std::cout << cian << "| " << reset << "Probabilidad de ganar: "
                << jugadores[i]->TipoProbabilidad[i] << "%\n";
    }

    std::cout << cian << "|--------------------------------------------"
              << reset << std::endl;
  }

  std::cout << cian << "+-------------------------------------------+" << reset
            << std::endl;
  std::cout << cian << "|            Fin de los resultados          |" << reset
            << std::endl;
  std::cout << cian << "+-------------------------------------------+" << reset
            << std::endl;
}

void Tablero() {
  // Imprimir la tabla de la ruleta

  const std::string negro = "\033[1;37m";     // Texto negro
  const std::string blanco = "\033[1;37m";    // Texto blanco
  const std::string fondoRojo = "\033[41m";   // Fondo rojo
  const std::string fondoNegro = "\033[40m";  // Fondo negro
  const std::string reset = "\033[0m";        // Restablecer colores

  std::cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--"
               "---+-----+-----+-----+\n";
  std::cout << "| " << fondoRojo << negro << "  3  " << reset;
  std::cout << "| " << fondoNegro << blanco << "  6  " << reset;
  std::cout << "| " << fondoRojo << negro << "  9  " << reset;
  std::cout << "| " << fondoRojo << blanco << " 12  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 15  " << reset;
  std::cout << "| " << fondoRojo << blanco << " 18  " << reset;
  std::cout << "| " << fondoRojo << negro << " 21  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 24  " << reset;
  std::cout << "| " << fondoRojo << negro << " 27  " << reset;
  std::cout << "| " << fondoRojo << negro << " 30  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 33  " << reset;
  std::cout << "| " << fondoRojo << negro << " 36  " << reset << "|\n";
  std::cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--"
               "---+-----+-----+-----+\n";
  std::cout << "| " << fondoNegro << blanco << "  2  " << reset;
  std::cout << "| " << fondoRojo << negro << "  5  " << reset;
  std::cout << "| " << fondoNegro << blanco << "  8  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 11  " << reset;
  std::cout << "| " << fondoRojo << negro << " 14  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 17  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 20  " << reset;
  std::cout << "| " << fondoRojo << negro << " 23  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 26  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 29  " << reset;
  std::cout << "| " << fondoRojo << negro << " 32  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 35  " << reset << "|\n";
  std::cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--"
               "---+-----+-----+-----+\n";
  std::cout << "| " << fondoRojo << negro << "  1  " << reset;
  std::cout << "| " << fondoNegro << blanco << "  4  " << reset;
  std::cout << "| " << fondoRojo << negro << "  7  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 10  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 13  " << reset;
  std::cout << "| " << fondoRojo << negro << " 16  " << reset;
  std::cout << "| " << fondoRojo << negro << " 19  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 22  " << reset;
  std::cout << "| " << fondoRojo << negro << " 25  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 28  " << reset;
  std::cout << "| " << fondoNegro << blanco << " 31  " << reset;
  std::cout << "| " << fondoRojo << negro << " 34  " << reset << "|\n";
  std::cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--"
               "---+-----+-----+-----+\n";
  std::cout << "\n\n";
}