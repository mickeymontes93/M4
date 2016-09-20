main{
	var.int numero1,numero2,temporal;
	console.write("Ingrese un numero entero a evaluar");
	console.read("%d",numero1);

	console.write("Ingrese otro numero entero a evaluar");
	console.read("%d",numero2);

	temporal := numero1;
	numero1 := numero2;
	numero2 := temporal;

	console.write("El numero 2 es %d y el numero 1 es %d", numero1,numero2);
}