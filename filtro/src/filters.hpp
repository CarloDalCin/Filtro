#pragma once

class Filters
{
    public:
    //ritorna vero se una lettera Maiuscola
    static bool isCapital(const char c) {
      return c>='A' && c<='Z';
    }

    //ritorna vero se una lettera minuscola
    static bool isLower(const char c) {
      return c>='a' && c<='z';
    }

    //ritorna vero se il carattere passato è una lettera M. o m.
    static bool isLetter(const char c) {
        return isCapital(c) || isLower(c);
    }

    // per verificare se c è una vocale
    static bool isVowel(const char c) {
      return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }

    // per verificare se c è una consonante
    static bool isConsonant(const char c) {
        return !isVowel(c) && isLetter(c);
    }

    //ritorna vero se è la rapresentazione ascii di un numero
    static bool isNumber(const char c) {
      return c>='0' && c<='9';
    }

    //ritorna vero se non ci sono c non è ne un numero ne una lettera (quindi un simbolo)
    static bool isSymbol(const char c) {
      return !(isNumber(c) || isLetter(c));
    }
};
