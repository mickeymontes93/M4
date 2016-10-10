main{
    var.str d := "Soy una string";
    var.int a; 
    var.int b;
    var.int c := a + b; 
    c+=1.56;
    #Comentario
    if(c==4 && a <=2 || b>=0){
        var.bool banderita:= true;
        while(banderita){
            console.write("hola %s %d %f mundo");
            banderita:=false;
            console.write('adios %d mundo');
        }
    }
    #Otro comentario
    console.write("Prueba ");
    return;
}

