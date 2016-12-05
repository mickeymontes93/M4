MAIN{
	var.int opcion;
	var.int contador;
	var.int i;
	var.float test;
	contador := 0;
	i := 0;
	opcion := 1;
	test := 5;

	if(test < opcion)
	{
		for(contador := 0; contador < 10; contador++)
		{
			if (opcion > 5)
			{
				opcion := opcion + 1;			
			}
			else
			{
				opcion := opcion + 1;
			}
		}
		test := opcion;
	}
	if(test == opcion)
	{
		while(opcion != 0)
		{
			if (opcion > 5)
			{
				opcion := opcion - 1;			
			}
			else
			{
				opcion := opcion - 1;
			}
		}
		test := opcion;
	}
}