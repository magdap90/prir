package watki;
import java.util.Random;

interface Dane{
    static final int Ilosc_statkow=5; // sterowanie iloscia  
    static int ilosc_miejsc_max=10;
}

public class Watki implements Dane {
    static Statek statek;

    
    public static void main(String[] args)  {
        statek = new Statek(Ilosc_statkow);//,ilosc_miejsc);
      
        for(int i=0;i<ilosc_miejsc_max;i++)
        {
           
            new Port(i,statek).start();
        }      
    }
    
}
class Port extends Thread{
    
    private int numer;
    Random rand;
    
    Statek s;
    Port(int numer, Statek s)
    {
        this.numer=numer;
        rand=new Random();
        this.s = s;
    }
    
    public void run()
    {
        while(true)
        { 
        if(rand.nextInt(2)==1)
        {
           
             System.out.println("Dobra pogoda pozwolenie na start"); 
             s.dodaj_towar();
             s.start();
             if(rand.nextInt(2)==1)
                 s.okradziony(); 
             s.powrot();
        }
        else
        {
            System.out.println("Zla pogoda, nie mozna wyplynąc");
            s.powrot();
        }
        
        }
    }
}

class Statek implements Dane
{
    static int START=2;
    static int KONIEC=3;
    
   int pogoda;
   int ilosc_zajetych;
   int towar=0;
   
    Statek(int ilosc_zajetych)
    {
        this.ilosc_zajetych=ilosc_zajetych;
    }
    synchronized int start()
    {
      ilosc_zajetych--;
      System.out.println("Wypływ z towarem statek ");
      return (START);
    }
    synchronized int powrot()
    {
       ilosc_zajetych++;
      System.out.println("Pozwolenie na powrot statku:  ");
      return KONIEC; 
    }
    synchronized void dodaj_towar()
    {
        towar ++;
        System.out.println("Dodanie towaru: " + towar); 
    }
    
    synchronized void okradziony()
    {
        towar --;
        System.out.println("Kradzierz");
        System.out.println("Ilosc towatu teraz: " + towar);
        
    }
}