package prir_lab2;

class M_Prostokatow extends Thread {
    double xp, xk, n;
    public M_Prostokatow(double xp1, double xk1, int ile_watkow)
    {
        xp=xp1;
        xk=xk1;
        n=ile_watkow;
    }
    
      double func(double x)
    {
        return (Math.cos((x*x)+0.7)/(1.1+Math.sin((x*0.6)+0.2)));
    }
      
    public void run(){
        double dx, calka;
        dx = (xk - xp) / (double)n;
 
        calka = 0;
        for (int i=1; i<=n; i++) {
                calka += func(xp + i * dx);
        }
        
        calka *= dx;
        System.out.println("Wartosc metoda prostokatow " + calka);
    }
}
class M_Trapezow extends Thread {
   
    double xp, xk, n;
    public M_Trapezow(double xp1, double xk1, int ile_watkow)
    {
        xp=xp1;
        xk=xk1;
        n=ile_watkow;
    }
   
  double func(double x)
    {
        return (Math.cos((x*x)+0.7)/(1.1+Math.sin((x*0.6)+0.2)));
    }
 
  public void run()
  {
      double dx, calka = 0;
        dx= (xk - xp)/n;
       
        for( int i =1; i<n; i++)
        {
            calka += func(xp+i*dx);
        }
        calka +=(func(xp)+func(xk))/2;
        calka *=dx;
        System.out.println("Wartosc metoda trapezow "+calka);
  }
}
class M_Simpsona extends Thread {
    double xp, xk, n;
        public M_Simpsona(double xp1, double xk1, int ile_watkow)
    {
        xp=xp1;
        xk=xk1;
        n=ile_watkow;
    }
   
  double func(double x)
    {
        return (Math.cos((x*x)+0.7)/(1.1+Math.sin((x*0.6)+0.2)));
    }
  
  public void run ()
  {
    double dx,calka, s, x;
        dx=(xk - xp)/n;
        calka = 0;
        s=0;
        for(int i=1; i<n;i++){
        x = xp+ i*dx;
        s +=func(x-dx/2);
        calka +=func(x);
    }
        s+=func(xk-dx/2);
        calka = (dx/6)*(func(xp)+func(xk)+2*calka+4*s);
        System.out.println("Wartosc metoda simpsona "+calka);
  }
    
}

public class PRIR_lab2 {
    
    public static void main(String[] args) {

        M_Trapezow t1 = new M_Trapezow(1.5,2.8, 1000);
        M_Trapezow t2 = new M_Trapezow(2.1,3.5, 1000);
        M_Trapezow t3 = new M_Trapezow(1.7,2.6, 1000);
        t1.start();
        t2.start();
        t3.start();
        
        M_Simpsona s1 = new M_Simpsona(1.5,2.8, 10);
        M_Simpsona s2 = new M_Simpsona(0.9,3.1, 10);
        M_Simpsona s3 = new M_Simpsona(1.1,4.2, 10);
        s1.start();
        s2.start();
        s3.start();
        
        M_Prostokatow p1 = new M_Prostokatow(1.5,2.8, 100000);
        M_Prostokatow p2 = new M_Prostokatow(0.4,3.7, 100000);
        M_Prostokatow p3 = new M_Prostokatow(2.2,5.4, 100000);
        p1.start();
        p2.start();
        p3.start();
        
    }
}