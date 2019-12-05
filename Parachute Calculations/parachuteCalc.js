
let m = 0.350;
let g = 9.81;
let cd = 0.75;
let p = 1.225;
let v = 10;

function setup() {
    let A = ( 2 * m * g) / (cd * p * (v*v)) / 8;

    let angle = 1/8 * PI;

    let h = sqrt(A / tan(angle));
    let s = A / h;
    let r = h / cos(angle);

    // Calculate to cm
    print ("A (m2) = " + A + " m2");
    print ("h (cm) = " + h * 100 + " cm");
    print ("s (cm) = " + s * 100 + " cm");
    print ("s totaal (cm) = " + s * 100 * 2 + " cm");
    print ("r (cm) = " + r * 100 + " cm");


    

}