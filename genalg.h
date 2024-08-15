// Algoritmo de congruencia lineal
class genAlg {
private:
    long long m; // módulo
    long long a; // multiplicador
    long long c; // incremento
    long long seed; // semilla

public:
    genAlg(long long _m, long long _a, long long _c, long long _seed)
            : m(_m), a(_a), c(_c), seed(_seed) {}

    // Genera el siguiente número aleatorio
    long long next() {
        seed = (a * seed + c) % m;
        return seed;
    }

    // Genera un número aleatorio entre min y max (inclusive)
    int nextInRange(int min, int max) {
        return min + next() % (max - min + 1);
    }
};
