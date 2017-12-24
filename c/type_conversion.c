#include <type_conversion.h>
#include <str.h>
#include <state_machine.h>

static unsigned long Dec[] RODATA = {		//este array de long se usa para las restas suscesivas en las funciones de conversion de long a BCD y de char a BCD. Notar que no banca el maximo de los long, porque por ahora no se necesita, en caso de requerir habra que seguir agragando potencias de 10 a este array///
 		            	   10000000, 	//convierte hasta 99 999 999 y yo necesito convertir 24 bits para los codigos del RC que va de 0 a 2^24 =16 777 215
 		            	   1000000, 
 		            	   100000,
 		            	   10000,
 	        	    	   1000,
 		        	   100,
	 		           10,
			    	   1
			   	   };
static unsigned long Hex[] RODATA = {		//este array de long se usa para las restas suscesivas en las funciones de conversion de long a BCD y de char a BCD. Notar que no banca el maximo de los long, porque por ahora no se necesita, en caso de requerir habra que seguir agragando potencias de 10 a este array///
 		        	   0x100000,
	 		           0x010000,
 		            	   0x001000,
 	        	    	   0x000100,
 		        	   0x000010,
	 		           0x000001,
			   	   };
	
unsigned char* Long2Bcd(unsigned char* Bcd,unsigned long Bin)	//convierte un Long (OJO de 22bits y <  a 4 99999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
 unsigned char i;													//contador auxiliar...
 unsigned long Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
 for (i=0;i<8;i++)	
  for(Bcd[i]='0';Aux=-Dec[i],(Aux+=Bin)<Bin;Bcd[i]++)		//se inicializa la posicion a guardar;mientras la resta del binario con la tabla que se guarda en aux sea menor que el binario mismo;incrementar el BCD y...
    Bin=Aux;												//como no se paso bin pasa a ser aux que ahora es menor que el original. Notar que se destruye bin...
 return Bcd;
}
unsigned char* Int2Bcd(unsigned char* Bcd,unsigned int Bin)	//convierte un Long (OJO menor a 99999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
	unsigned char i;													//contador auxiliar...
	unsigned int Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
	for (i=0;i<5;i++)	
	 for(Bcd[i]='0';Aux=-Dec[i+3],(Aux+=Bin)<Bin;Bcd[i]++)		//se inicializa la posicion a guardar;mientras la resta del binario con la tabla que se guarda en aux sea menor que el binario mismo;incrementar el BCD y...
	   Bin=Aux;												//como no se paso bin pasa a ser aux que ahora es menor que el original. Notar que se destruye bin...
	return Bcd;
}
unsigned char* Int2_3Digit_Bcd(unsigned char* Bcd,unsigned int Bin)	//convierte un Long (OJO menor a 999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
 unsigned char i;													//contador auxiliar...
 unsigned int Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
 for (i=0;i<3;i++)	
  for(Bcd[i]='0';Aux=-Dec[i+5],(Aux+=Bin)<Bin;Bcd[i]++)			//se inicializa la posicion a guardar;mientras la resta del binario con la tabla que se guarda en aux sea menor que el binario mismo;incrementar el BCD y...
    Bin=Aux;								//como no se paso bin pasa a ser aux que ahora es menor que el original. Notar que se destruye bin...
 return Bcd;
}
unsigned char* Int2_4Digit_Bcd(unsigned char* Bcd,unsigned int Bin)	//convierte un Long (OJO menor a 999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
 unsigned char i;													//contador auxiliar...
 unsigned int Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
 for (i=0;i<4;i++)	
  for(Bcd[i]='0';Aux=-Dec[i+4],(Aux+=Bin)<Bin;Bcd[i]++)			//se inicializa la posicion a guardar;mientras la resta del binario con la tabla que se guarda en aux sea menor que el binario mismo;incrementar el BCD y...
    Bin=Aux;								//como no se paso bin pasa a ser aux que ahora es menor que el original. Notar que se destruye bin...
 return Bcd;
}
unsigned char* Char2Bcd(unsigned char* Bcd, unsigned char Bin)						//convierte un Char a BCD...
{
 unsigned char Aux,i;										//un auxiliar para recorrer y otro para ir tomando nota de las restas...
  for(i=0;i<3;i++)										//se recorre 3 veces porque banca hasta 255 que ocupa 3 bytes...
   for(Bcd[i]='0';Aux=-Dec[i+5],(Aux+=Bin)<Bin;Bcd[i]++)	//se inicializa la posicion a grabar el BCD en cuestion; se resta bin con la parte de la tabla que corresponde y se guarda en aux; se incrementa el BCD en cuestion y...
     Bin=Aux; 											//como no se paso bin pasa a ser aux...
 return Bcd;
}
unsigned char* Char2_2Digit_Bcd(unsigned char* Bcd, unsigned char Bin)						//convierte un Char a BCD...
{
 unsigned char Aux,i;
  for(i=0;i<2;i++)
   for(Bcd[i]='0';Aux=-Dec[i+6],(Aux+=Bin)<Bin;Bcd[i]++)	
     Bin=Aux; 											//como no se paso bin pasa a ser aux...
 return Bcd;
}

unsigned long Dec_Bcd2Long(unsigned char* Bcd)
{
 unsigned char i;
 unsigned long Aux=0;						//contador auxiliar...
 for (i=0;i<8;i++) Aux+=(Bcd[i]-'0')*Dec[i];
 return Aux;
}
unsigned int Dec_Bcd2Int(unsigned char* Bcd)
{
 unsigned char i;
 unsigned int Aux=0;						//contador auxiliar...
 for (i=0;i<5;i++) Aux+=(Bcd[i]-'0')*Dec[i+3];
 return Aux;
}
unsigned char Dec_Bcd2Char(unsigned char* Bcd)
{
 unsigned char i;
 unsigned char Aux=0;						//contador auxiliar...
 for (i=0;i<3;i++) Aux+=(Bcd[i]-'0')*Dec[i+5];
 return Aux;
}
//----------------------------------------------------------------------------------------------------
void Rigth_Roll(unsigned char *Data,unsigned char Times,unsigned char Length)
{
 unsigned char i,Bit0,Bit7;
 while(Times--)
  for(Bit7=Data[Length-1]<<7,i=0;i<Length;i++)
   {
    Bit0=Data[i]<<7;
    Data[i]>>=1;
    Data[i]|=Bit7;
    Bit7=Bit0;
   }
}
void Left_Shift(unsigned char *Data,unsigned char Times,unsigned char Length)
{
 unsigned char i,Bit7,Bit0;
 while(Times--)
  for(Bit0=0,i=Length;i--;)
   {
    Bit7=Data[i]>>7;
    Data[i]<<=1;
    Data[i]|=Bit0;
    Bit0=Bit7;
   }
}
//----------------------------------------------------------------
unsigned char* String2Hex_Bcd(unsigned char* Bcd,unsigned char* String,unsigned char Length)
{
 while(Length--) Char2Hex_Bcd(Bcd+2*Length,String[Length]);
 return Bcd;
}

unsigned char* Char2Hex_Bcd(unsigned char* Bcd,unsigned char Bin)	//convierte un Long (OJO menor a 99999, si se requiere mayor rango solamente se deberan agregar valores al array de const de arriba y reservar lugar para mas BCD claro) en su BCD y coloca cada BCD en 1 byte entero (no se puso 2 por bytes porque fue menos eficiente en espacio de flash y despues para procesarlos con los bits de paridad pesaba mas, el calsico Flash Vs Ram)...
{
 unsigned char i;													//contador auxiliar...
 unsigned int Aux;													//es un long auxiliar para acordarse del valor de una resta de longs...
 for (i=0;i<2;i++)	
  for(Bcd[i]=0;Aux=-Hex[i+4],(Aux+=Bin)<Bin;Bcd[i]++)		//se inicializa la posicion a guardar;mientras la resta del binario con la tabla que se guarda en aux sea menor que el binario mismo;incrementar el BCD y...
    Bin=Aux;												//como no se paso bin pasa a ser aux que ahora es menor que el original. Notar que se destruye bin...
 for(i=0;i<2;i++) Bcd[i]+=(Bcd[i]>9)?('A'-10):'0';
 return Bcd;
}
unsigned char* Int2Hex_Bcd(unsigned char* Bcd,unsigned int Bin)	
{
	return String2Hex_Bcd(Bcd,(unsigned char*)&Bin,2);
}
//----------------------------------------------------------------
unsigned char* Signed_Int2_2Dec_Fix_Point_Bcd(unsigned char* Bcd,signed int Bin)	//convierte un entero pero lo considera como punto fijo y lo pasa a bcd considerando 2 decimales mas signo, del tipo "+655.35"
{
 String_Copy((unsigned char*)"+655.35",Bcd,7);			//copia el formato en el destino
 if(Bin<0) {Bcd[0]='-';Bin=-Bin;}		//si el valore es negativo, se cambia el signo y se complementa el daro para poder convertir a bcd
 Int2Bcd(Bcd+1,Bin);				//convioerte a bcd y pisa la respuesta salteando la primera posicion reservada para el signo...
 Shift_String2Rigth(Bcd+4,2,1);			//hace lugar para poner el punto decimal..
 Bcd[4]='.';					//agrega el puntito...
 return Bcd;
}
//----------------------------------------------------------------
unsigned char*	Replace_Zero2Space(unsigned char* Buf,unsigned char Length)
{
	unsigned char i;
	for(i=0;i<Length && Buf[i]=='0';i++) Buf[i]='/';		//ceros a la izquierda afuera.. excepto el primero...
	return Buf;
}

