#include <cstddef>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <fstream>
//file contentente la classe con i diversi tipi di filtri
#include "filters.hpp"

using std::cout;
using std::endl;
using std::cerr;

typedef bool(*pf_filter)(const char);
typedef void(*pf_logic_func)(std::ifstream&, std::ofstream&, pf_filter);

void checkParam(int argc, char** argv);
void removeFiltered(std::ifstream& fIn, std::ofstream& fOut, pf_filter filter);
void leaveFiltered(std::ifstream& fIn, std::ofstream& fOut, pf_filter filter);
std::string path(const char& arg);
std::fstream openFile(const std::string& path, std::ios_base::openmode);

//associatore tra parametri e il tipo di filtro che devono fare
//per ogni carattere inserito posso avere una funzione associata
std::unordered_map<char, pf_filter> filters
{
  {'M', &Filters::isCapital},    // filtro per le Maiuscole
  {'m', &Filters::isLower},      //   "    per le minuscole
  {'l', &Filters::isLetter},     //   "    per le lettere
  {'n', &Filters::isNumber},     //   "    per i numeri
  {'s', &Filters::isSymbol},     //   "    per i simboli
  {'v', &Filters::isVowel},      //   "    per le vocali
  {'c', &Filters::isConsonant}   //   "    per le consonanti
  // ..........
};

//logica che gestisce cosa fare con il filtro
std::unordered_map<char, pf_logic_func> logic_func
{
  {'^', &leaveFiltered}
};


int main(int argc, char** argv) {
  /*
   * formato: filtro "fileInput" "fileOutput" -param
   *  argv      [0]      [1]          [2]       [3]
  */

  checkParam(argc, argv);

  //[todo] gestire la possibilita' di un unico file I/O
  //file fi input...
  //aquisizione del percorso del file di input
  const std::string pathIn = path(*argv[1]);
  //creazione dello stream di input con controllo annesso
  std::fstream fIn = openFile(pathIn, std::ios_base::in);

  //file di output...
  const std::string pathOut = path(*argv[2]);
  std::fstream fOut = openFile(pathOut, std::ios_base::out);

  std::fstream *pFIn = &fIn, *pFOut = &fOut;

  //puntatori a funzione
  pf_logic_func func;
  pf_filter filter;

  char* param = &argv[3][1]; //primo parametro (esclusione del carattere '-')
  size_t paramSize = strlen(argv[3]); //dimensione della stringa (senza il carattere '\0')
  char* ultimateParam = &argv[3][paramSize]; //indirizzo del ultimo carattere della stringa
  //ripetizione del ciclo per ogni tipo di filtro da applicare specificato nei parametri
  for(; param<ultimateParam; ++param) {
    pFIn->seekg(0);
    pFOut->seekp(0);
    if(logic_func.find(*param) != logic_func.end() && param+1<=ultimateParam) {
      if(logic_func.find(*param+1) != logic_func.end()) {
        cerr << "Errore nel inserimento dei parametri\n";
      } else
        func = logic_func[*param++];
    } else
      func = &removeFiltered; //funzione di default

    //scelta del filtro
    if(filters.find(*param) != filters.end())
      filter = filters[*param];
    else
      cout << "Parametro: " << *param << " non esistente\n";

    //chiamata del puntatore a funzione che gestisce la logica dei filtri
    func((std::ifstream&)*pFIn, (std::ofstream&)*pFOut, filter);

    //rende il file di output il nuovo file di input cosicche'
    //si possa applicare un nuovo filtro a quello gia' applicato
    pFIn = pFOut;
  }

  fIn.close();
  fOut.close();
}

void checkParam(int argc, char** argv) {
  std::string format = "formato: \'filtro \"fileIn.txt\" \"fileOut.txt\" -parametri\'";


  // possibile miglioramento con lutilizzo di uno switch()



  if(argc == 2 && ( !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h") ) ) {
    cout << "prova\n";
    exit(1);
  }

  //controllo il numero di parametri passati e cosa si vuole filtrare
  if(argc!=4) {
    cerr << "Il numero di parametri inseriti non corrisponde\n";
    cout << format << endl;
    exit(1);
  }

  if(strlen(argv[3]) <= 1 || *argv[3]!='-') {
    cerr << "Il formato dei parametri non corrisponde\n";
    cout << format << endl;
    exit(1);
  }
}

//omette dal output cio che il filtro identifica
void removeFiltered(std::ifstream& fIn, std::ofstream& fOut, pf_filter filter) {
  char c;
  while(fIn.get(c)) {
    if(!filter(c))
      fOut << c;
  }
}

void leaveFiltered(std::ifstream& fIn, std::ofstream& fOut, pf_filter filter) {
  char c;
  while(fIn.get(c)) {
    if(filter(c))
      fOut << c;
  }
}

//se largomentro passato è tra il simbolo " questo viene rimosso e ritornata la nuova str
//altrimenti viene ritornata la stringa di arg
std::string path(const char& arg) {
  std::string str(&arg);
  if(std::strchr(&arg, '"'))
    str.resize(1, str.size()-1);
  return str;
}

std::fstream openFile(const std::string& path, std::ios_base::openmode mode) {
  std::fstream f(path, mode);
  if(!f.is_open()) {
    cerr << "Errore nel apertura del file: " << path << endl;
    exit(1);
  }
  return f;
}


/*
 * [todo]
 * 1) implementare la possibilita di inserire solo un file che sia contemporaneamente di input che di output (std::ios::openmode mode = ios_base::in | ios_base::out)
 * 2) gestire l'ecezzione nei casi ambigui che si possono verificare con la gestione della pf_logic_func nella fase di aquisizione dei parametri (es: -^^m, -vs^)
 * 3) aggiungere piu' filtri, anche di stringe intere e non solo di caratteri, mettere i tipi ti funzione puntatore che riguardano le funzioni del filtro come pf_filtro al interno della classe Filters
 * 4) completare helper come parametro
 *
 */

