import java.util.*;

class Point {
    private Integer x;
    private Integer y;

    public Point(Integer x, Integer y){
        this.x = x;
        this.y = y;
    }

    public Integer getX(){ return this.x; }
    public Integer getY(){ return this.y; }
    public void setX(Integer x){ this.x = x; }
    public void setY(Integer y){ this.y = y; }
}


public class Tarea10 {
    public static void main(String[] args){
        Scanner scan = new Scanner(System.in);

        //-- Declare variables
        Integer nConjunto1 = new Integer(0);
        ArrayList<Point> conjunto1 = new ArrayList<Point>();
        Integer nConjunto2 = new Integer(0);
        ArrayList<Point> conjunto2 = new ArrayList<Point>();

        //-- Read phase
        nConjunto1 = scan.nextInt();
        for(int i=0; i<nConjunto1; i++) conjunto1.add( new Point(scan.nextInt(), scan.nextInt()) );
        nConjunto2 = scan.nextInt();
        for(int i=0; i<nConjunto2; i++) conjunto2.add( new Point(scan.nextInt(), scan.nextInt()) );

        //-- Process stuff
        
    }
}