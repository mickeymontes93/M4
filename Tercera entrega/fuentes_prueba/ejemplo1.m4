MAIN{
#DECLARACION
    var.int 111;
    var.str d;
    var.str a;
    var.int b;
    var.int x;
    var.int e;
    var.float c;
    var.file varFILE;
    var.bool banderita;
    arr.int arrEnteros;
    arr.str arrStrings;
    var.str a;

#FUNCION
    func.void funcPrueba();
    func.int ;
   

#INSTRUCCION
    banderita:= true;
    a:= #"Soy una string";
    c:= 1.56;
    d:=10;
    arrEnteros := [10,20,30,40];
    arrStrings := ["hola","adios","string"];
    for(e:=0;e<5;+1){
		console.write("%d",e);
	}
}

#FUNCION INSTRUCCION
func.void funcPrueba(){
	b:=2;
        if(c==4 && b<=2 || b>=0){
		do{
               	 console.write("%s",a);
               	 banderita:=false;
               	 console.write('adios %s',d);
		}while(banderita)
	}
	
	switch b {
		#case (b==2)
		{
			b:=3;
			#break;
		}
        case(b==3)
        {
            break;
        }
		#default 
		#{
			b:=0;
			break;
		}
	}
}