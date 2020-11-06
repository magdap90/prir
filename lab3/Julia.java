package julia;
import java.util.concurrent.*;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.io.File;
import java.io.IOException;

class JuliaTask extends Thread{
    int [][] pixels;
    int startX, endX, startY, endY;
    
    JuliaTask(int [][] pixels, int startX, int endX, int startY, int endY) {
        this.pixels = pixels;
        this.startX = startX;
        this.endX = endX;
        this.startY = startY;
        this.endY = endY;
    }
    
    @Override
    public void run() {
        for (int y = startY; y < endY; ++y) {
            for (int x = startX; x < endX; ++x) {
                pixels[y][x] = kalkulatorKoloru(x, y);
            }
        }
    }
    
    /*
    Ta metoda zwraca kolor RGB punktu w (x, y).
     Jeśli punkt nie należy do zbioru Julii, zwraca 0, czyli czarny.
    */
    int kalkulatorKoloru(int x, int y) {
        double cx = -0.4; // ustawianie parametru np -0.8  lub 0.285
        double cy = +0.6; // ustawianie parametru np + 0.156 lub +0.01
        double R  = 1.6;  // choose R > 0 such that R**2 - R >= sqrt(cx**2 + cy**2)
        double width  = pixels[0].length;
        double height = pixels.length;

        double zx = (x / width) * 2 * R - R;  // skalowana współrzędna x piksela (miedzy -R i R)
                                              // zx reprezentuje rzeczywista czesc z 
        double zy = (y / height) * 2 * R - R;  // skalowana wspolrzedna y (miedzy -R i R)
                                               // zy reprezentuje urojoną czesc z 
        int iteracje = 0;
        int maxIteracji = 1000;
        while (zx * zx + zy * zy < R*R && iteracje < maxIteracji) {
            double xtemp = zx * zx - zy * zy;
            zy = 2 * zx * zy + cy;
            zx = xtemp + cx;
            ++iteracje;
        }
        int kolor = 0;  // czarne dla punktów, dla których iteracje >= maxIteracji
        if (iteracje < maxIteracji) {  // (x, y) należy do zbioru Julia
            float dist = iteracje / (float)maxIteracji;
            byte r = kalkKolorComponent(dist, 100, 1./4, 2.5);
            byte g = kalkKolorComponent(dist, 50, 1./2, 2.5);
            byte b = kalkKolorComponent(dist, 50, 3./4, 2.5);
            kolor = (r << 16) | (g << 8) | b;
        }
        return kolor;
    }
    
    static byte kalkKolorComponent(double x, double w, double c, double p) {
        return (byte)(Math.exp( -w * Math.pow(Math.abs(x - c), p) ) * 255);
    }
}

public class Julia {
    
    public static void main(String [] args) {
        final int N = 2000;
        int [][] pixels = new int[N][N];
        
        long parStartTime = System.nanoTime();
        
        ForkJoinPool pool = ForkJoinPool.commonPool();  // uzycie wbudowanej funkcji do sprawdzania co w puli
        boolean sequential = false;
        // Jeśli sekwencyjność jest prawdziwa, używamy tylko 1 o rozmiarze NxN
        int tile = sequential ? N : 100;
        // Utwórz zadanie dla każdego kafelka
        for (int x = 0; x < N; x += tile) {
            for (int y = 0; y < N; y += tile) {
                pool.submit(new JuliaTask(pixels, x, x + tile, y, y + tile));// wywolanie watku 
            }
        }
        // Poczekaj na zakończenie zadań - ustaw limit czasu na 1 minutę:
        ForkJoinPool.commonPool().awaitQuiescence(1L, TimeUnit.MINUTES);
        
        long parElapsed = System.nanoTime() - parStartTime;
        System.out.format("Czas wykonania : %f sec.\n", parElapsed * 1e-9);
        
        long imageWriteStartTime = System.nanoTime();
        BufferedImage image = new BufferedImage(N, N, BufferedImage.TYPE_INT_RGB);
        for (int y = 0; y < N; ++y) {  // Skopiuj wiersze pikseli do obrazu
            image.setRGB(0, y, N, 1 /* jeden rzad */, pixels[y], 0, 0);
        }
        File outputfile = new File("julia.jpg");
        try {
            ImageIO.write(image, "jpg", outputfile);
        } catch(IOException e) {
            e.printStackTrace();
        }
        long imageWriteElapsed = System.nanoTime() - imageWriteStartTime;
        System.out.printf("Obraz zapisany w %.2f sec.\n", imageWriteElapsed * 1e-9);
    }
}