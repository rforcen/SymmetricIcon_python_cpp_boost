#ifndef SymmetricIconH
#define SymmetricIconH

#include "complex.h"

template <class T>
class SymmetricIcon {

    static const int maxColors=2112;
    
public:
    T _preset[36][7] = {
        {1.56, -1, 0.1, -0.82, -0.3, 3, 1.7},
        {-1.806, 1.806, 0, 1.5, 0, 7, 1.1}, {2.4, -2.5, -0.9, 0.9, 0, 3, 1.5},
        {-2.7, 5, 1.5, 1, 0, 4, 1}, {-2.5, 8, -0.7, 1, 0, 5, 0.8},
        {-1.9, 1.806, -0.85, 1.8, 0, 7, 1.2},
        {2.409, -2.5, 0, 0.9, 0, 4, 1.4},
        {-1.806, 1.807, -0.07, 1.08, 0, 6, 1.2},
        {-2.34, 2.2, 0.4, 0.05, 0, 5, 1.2},
        {-2.57, 3.2, 1.2, -1.75, 0, 36, 1.2}, {-2.6, 4, 1.5, 1, 0, 12, 1.1},
        {-2.2, 2.3, 0.55, -0.90, 0, 3, 1.3},
        {-2.205, 6.01, 13.5814, -0.2044, 0.011, 5, 0.8},
        {-2.7, 8.7, 13.86, -0.13, -0.18, 18, .8},
        {-2.52, 8.75, 12, 0.04, 0.18, 5, .8},
        {2.38, -4.18, 19.99, -0.69, 0.095, 17, 1},
        {2.33, -8.22, -6.07, -0.52, 0.16, 4, .8},
        {-1.62, 2.049, 1.422, 1.96, 0.56, 6, 1},
        {-1.89, 9.62, 1.95, 0.51, 0.21, 3, .6},
        {-1.65, 9.99, 1.57, 1.46, -0.55, 3, .8}, {-2.7, 5, 1.5, 1, 0, 6, 1},
        {-2.08, 1, -.1, .167, 0, 7, 1.3}, {1.56, -1, .1, -.82, .12, 3, 1.6},
        {-1.806, 1.806, 0, 1, 0, 5, 1.1}, {1.56, -1, .1, -.82, 0, 3, 1.3},
        {-2.195, 10, -12, 1, 0, 3, .7}, {-1.86, 2, 0, 1, .1, 4, 1.2},
        {-2.34, 2, .2, .1, 0, 5, 1.2}, {2.6, -2, 0, .5, 0, 5, 1.3},
        {-2.5, 5, -1.9, 1, .188, 5, 1}, {2.409, -2.5, 0, .9, 0, 23, 1.2},
        {2.409, -2.5, -.2, .81, 0, 24, 1.2}, {-2.05, 3, -16.79, 1, 0, 9, 1},
        {-2.32, 2.32, 0, .75, 0, 5, 1.2}, {2.5, -2.5, 0, .9, 0, 3, 1.3},
        {1.5, -1, .1, -.805, 0, 3, 1.4}};
    
    T*preset=(T*)_preset;
    int sw, sh; // screen w,h MUST be a power of two
    int*screen;
    int iter; // current iteration
    int npr; // preset number
    T lambda, alpha, beta, gamma, omega; // icon parameters
    unsigned sym;
    T apcx, apcy, rad;
    T scale; // 1.5;
    int colors[maxColors];
    int speed;
    short*icon;
    int nPreset;
    T x, y;
    int k;
    bool inGenerate=false;

    SymmetricIcon(int w, int h, int prSet = 0) {

        initPython();

        // lambda, alpha, beta, gamma, omega, symmetry, scale
       
        nPreset = sizeof(_preset) / (7 * sizeof(T));
        
        sw = sh = 0; // screen w,h MUST be a power of two
        screen = NULL;
        icon = NULL;
        npr = 0; // preset number
        
        speed = 100;

        setPreset(prSet);
        setColors(0);
        setSize(w,h);
    }

    static np::ndarray gen(int w, int h, int prSet, int iters) {
        SymmetricIcon symicon(w, h, prSet);
        return symicon.generate_image(iters);
    }


    np::ndarray getimage_np() { // return numpy array to direct plot, image->numpy ARGB [..., :3] -> RGB
        return np::from_data(screen,            // data -> image
            np::dtype::get_builtin<byte>(),     // dtype -> byte
            p::make_tuple(sh, sw, 4),           // shape -> h x w x 4
            p::make_tuple(sw*4, 4, 1), p::object());    // stride in bytes [1,1,1,1] (4) each row = w x 4
    }

    np::ndarray generate_image(int iters) {
        for (int i=0; i<iters; i++)
            generate(iters);
        return getimage_np();
    }

    static void initPython() {
        Py_Initialize(); // init boost & numpy boost
        np::initialize();
    }

    void setSize(int w, int h) {
        
        waitGeneration();
        
        sw = w;
        sh = h;
        int size=w*h, sz2=(sw+2) * (sh+2);
        if (screen) delete []screen;
        if (size) screen = new int[sz2];
        if (icon)  delete icon;
        if (size) icon = new short[sz2];
        speed = 100;

        reset();
    }
    
    inline int coord1(int x, int y) { // convert 2d -> 1d coord
        return x + y * sw;
    }
    bool iterMod(int mod) {
        return iter % mod == 0;
    }
    inline int argb(int a, int r, int g, int b) {
        return a << 24 | b << 16 | g << 8 | r;
    }
    inline int makeColor(int r, int g, int b) {
        return b << 16 | g << 8 | r | 0xff000000;
    }
    inline int makeColor(int a, int r, int g, int b) {
        return a << 24 | b << 16 | g << 8 | r;
    }
    
    void setColors(int colorIx) {
        int i, j, localColor;
        
        colorIx %= 12;
        
        if (colorIx > 5) {
            switch (colorIx) {
                case 6:  for (int i = 0; i < 256; i++) colors[(i + 64)] = makeColor(255, 255 - i, 255 - i, 255); break;
                case 7:  for (int i = 0; i < 256; i++) colors[(i + 64)] = makeColor(255 - i, 255, 255);       break;
                case 8:  for (int i = 0; i < 256; i++) colors[(i + 64)] = makeColor(255 - i, 255, 255 - i);   break;
                case 9:  for (int i = 0; i < 256; i++) colors[(i + 64)] = makeColor(255, 255, 255 - i);       break;
                case 10: for (int i = 0; i < 256; i++) colors[(i + 64)] = makeColor(255, 255 - i, 255 - i);   break;
                case 11: for (int i = 0; i < 256; i++) colors[(i + 64)] = makeColor(255, 255 - i, 255);       break;
            }
            for (i = 0; i < 64; i++)   colors[i] = makeColor(4 * i, 4 * i, 4 * i);
            for (j = 0; j < 5; j++) {
                for (i = 0; i < 256; i++)
                    colors[(320 + j * 256 + i)] = getRainbow((colorIx + j) % 6, i);
            }
            for (i = 0; i < 256; i++) {
                localColor = getRainbow((colorIx - 1) % 6, i);
                colors[(1600 + 2 * i)] = localColor;
                colors[(1601 + 2 * i)] = localColor;
            }
        }
        else {
            switch (colorIx) {
                case 0:
                    for (i = 0; i < 64; i++) colors[i] = makeColor(0, 0, 4 * i);
                    for (i = 0; i < 256; i++) {
                        localColor = makeColor(255, i, 255);
                        colors[(1344 + 3 * i)] = localColor;
                        colors[(1345 + 3 * i)] = localColor;
                        colors[(1346 + 3 * i)] = localColor;
                    }
                    break;
                case 1:
                    for (i = 0; i < 64; i++) colors[i] = makeColor(0, 4 * i, 4 * i);
                    for (i = 0; i < 256; i++) {
                        localColor = makeColor(i, i, 255);
                        colors[(1344 + 3 * i)] = localColor;
                        colors[(1345 + 3 * i)] = localColor;
                        colors[(1346 + 3 * i)] = localColor;
                    }
                    break;
                case 2:
                    for (i = 0; i < 64; i++) colors[i] = makeColor(0, 4 * i, 0);
                    for (i = 0; i < 256; i++) {
                        localColor = makeColor(i, 255, 255);
                        colors[(1344 + 3 * i)] = localColor;
                        colors[(1345 + 3 * i)] = localColor;
                        colors[(1346 + 3 * i)] = localColor;
                    }
                    break;
                case 3:
                    for (i = 0; i < 64; i++) colors[i] = makeColor(4 * i, 4 * i, 0);
                    for (i = 0; i < 256; i++) {
                        localColor = makeColor(i, 255, i);
                        colors[(1344 + 3 * i)] = localColor;
                        colors[(1345 + 3 * i)] = localColor;
                        colors[(1346 + 3 * i)] = localColor;
                    }
                    break;
                case 4:
                    for (i = 0; i < 64; i++)
                        colors[i] = makeColor(4 * i, 0, 0);
                    for (i = 0; i < 256; i++) {
                        localColor = makeColor(255, 255, i);
                        colors[(1344 + 3 * i)] = localColor;
                        colors[(1345 + 3 * i)] = localColor;
                        colors[(1346 + 3 * i)] = localColor;
                    }
                    break;
                case 5:
                    for (i = 0; i < 64; i++)
                        colors[i] = makeColor(4 * i, 0, 4 * i);
                    for (i = 0; i<255; i++) {
                        localColor = makeColor(255, i, i);
                        colors[(1344 + 3 * i)] = localColor;
                        colors[(1345 + 3 * i)] = localColor;
                        colors[(1346 + 3 * i)] = localColor;
                    }
            }
            for (j = 0; j < 5; j++) {
                for (i = 0; i < 256; i++)
                    colors[(64 + j * 256 + i)] =
                    getRainbow((colorIx + j) % 6, i);
            }
        }
    }
    
    int getRainbow(int x, int y) {
        switch (x) {
            case 0: return makeColor(0, y, 255);
            case 1: return makeColor(0, 255, 255 - y);
            case 2: return makeColor(y, 255, 0);
            case 3: return makeColor(255, 255 - y, 0);
            case 4: return makeColor(255, 0, y);
            case 5: return makeColor(255 - y, 0, 255);
        }
        return makeColor(0,0,0); // black
    }
    
    int getColor(int col) {
        col &= 0x00ffffff;
        if (col * speed > maxColors-1) {
            for (; (col * speed > 3071) && (speed > 3); --speed);
            return colors[maxColors-1];
        } else
            return colors[col * speed];
    }
    
    void reset() {

        speed = 100;

        apcx = (sw / 2.0);
        apcy = (sh / 2.0);
        rad = (apcx > apcy ? apcy : apcx);
        k = 0;
        x = 0.01;
        y = 0.003;
        
        for (int i = 0; i < sh * sw; i++) icon[i] =  0;
        
        for (int m = 0;  m < sw; m++)
            for (int n = 0; n < sh; n++)
                setPoint(m, n, getColor(icon[coord1(m, n)]));
        iter = 0;
    }
    
    void setPoint(int x, int y) {
        if (x >= 0 && y >= 0 && x < sw && y < sh)  {
            short &i = icon[coord1(x, y)];
            setPoint(x, y, getColor(i));
            i++;
            if (i > 12288) i = 8192;
        }
    }
    
    void setPoint(int x, int y, int color) {
        screen[coord1(x, y)] = color;
    }
    
    void setNextPreset() {
        setPreset((npr++) % nPreset);
    }
    
    void setPreset(int i) { // lambda, alpha, beta, gamma, omega, symmetry, scale
        i=i%nPreset;
        setParameters(preset[i*7 + 0], preset[i*7 + 1], preset[i*7 + 2],
                      preset[i*7 + 3], preset[i*7 + 4], (int) preset[i*7 + 5],
                      preset[i*7 + 6]);
        
    }

    void set_parameters(T lambda, T alpha, T beta, T gamma,
                       T omega, int sym, T scale) {

        this->lambda = lambda;
        this->alpha = alpha;
        this->beta = beta;
        this->gamma = gamma;
        this->omega = omega;
        this->sym = sym;
        this->scale = scale == 0 ? 1 : scale;
        reset();
    }
    
    void setParameters(T lambda, T alpha, T beta, T gamma,
                       T omega, int sym, T scale) {
        
        this->lambda = lambda;
        this->alpha = alpha;
        this->beta = beta;
        this->gamma = gamma;
        this->omega = omega;
        this->sym = sym;
        this->scale = scale == 0 ? 1 : scale;
        reset();
    }
    
    void setParameters(T*pars) { // lambda, alpha, beta, gamma, omega, symmetry, scale
        setParameters(pars[0], pars[1], pars[2], pars[3], pars[4], (int) pars[5], pars[6]);
    }
    
    T*getParameters() {
        static T rv[8];
        T _rv[8] = {lambda, alpha, beta, gamma, omega, (T)sym, scale};
        memcpy(rv, _rv, sizeof(rv));
        return rv;
    }
    
    void waitGeneration() {
        while(inGenerate);
    }

    bool generate(int modDisp) { // geenrate icon, runs in a thread in 'start'
        
        if(!screen) return false;
        
        const T MaxXY = 1e5;
        
        inGenerate=true;
        
        iter++;
        
        if (fabs(x) > MaxXY || fabs(y) > MaxXY)  reset(); // prevent overflow
        
        Complex<T> z(x,y), z1=z.pow(sym-1);
        
        // generate new x,y
        T sq = z.sqmag();
        
        T _re = x * z1.re - y * z1.im;
        T _im = lambda + alpha * sq + beta * _re;
        
        T xNew = _im * x + gamma * z1.re - omega * y;
        T yNew = _im * y - gamma * z1.im + omega * x;
        
        x = xNew;    y = yNew;
        
        if (k > 50) setPoint((int)(apcx + x * rad / scale), (int)(apcy + y * rad / scale));
        else k++;
        
        inGenerate=false;
        
        return (iter % modDisp) == 0;
    }
    
};
#endif
