#include <iostream>
#include <string.h>
#include <fstream>
#include <windows.h>
#define yuumi "cls"
#define yone "pause"


using namespace std; 

//--Nombres de los archivos utilizados
const char *nombre_archivo = "base_de_datos.dat";
const char *nombre_archivo_temp = "base_aux.dat";


void gotoxy(int x,int y){  
      HANDLE hcon;  
      hcon = GetStdHandle(STD_OUTPUT_HANDLE);  
      COORD dwPos;  
      dwPos.X = x;  
      dwPos.Y= y;  
      SetConsoleCursorPosition(hcon,dwPos);  
 } 


//--declaracion de campos de la estructura de datos
struct Funcion{
	int n;
	char espanol[20];
	char ingles[20];
	char funcion[1000];
}funciones;


//--Declaracion de funciones 
void agregar();
void leer();
void actualizar();
void eliminar();
void traduc_cod();
void funciones_archivo();
void getCodigo();
void traducir_codigo();
void lineacarga();


//--Funcion main
int main(){
	
	int op;
	
	do{
		system(yuumi);

		cout<<"---- 1. Ingresar la palabra y lo que realiza"<<endl;
		cout<<"---- 2. Ver palabras agregadas"<<endl;
		cout<<"---- 3. Actualizar palabra"<<endl;
		cout<<"---- 4. Eliminar palabra"<<endl;
		cout<<"---- 5. Traducir codigo"<<endl;
		cout<<"---- 0. Salir"<<endl;
		cout<<"\t\n ---- Digite su opcion: ";
		cin>>op; 
			
		system(yuumi);

		switch(op){
			case 1: agregar(); break;
 			case 2: leer(); break;
 			case 3: actualizar(); break;
 			case 4: eliminar(); break;
 			case 5: getCodigo(); break;
 			case 0: return 0; break;  
			default:cout<<"La opcion que ingreso no existe.";  
		}	
	}while(op!=0);

	return 0;
	system("pause");
}



//--Funcion guardar codigo para la traduccion.
void traducir_codigo(string cod_text){
	/* ios::app Todas las operaciones de salida se realizan al final del archivo
	  añadiendo el contenido al contenido actual del archivo.
	  
	  Usamos ofstream porque nos permite escribir literalmente le cadena en un archivo
	*/
	ofstream archivo ("cod_orginal.txt", ios::app); 
	  if (archivo.is_open()){								 														  
	    archivo << cod_text; //escribiendo
	    archivo.close();
	  }else{
	   cout << "error";
	  }
	  
}

//Funcion para obtener el codigo y delimitacion
void getCodigo(){
	
	string linea;

	cin.ignore();
	cout<<"______________________________________________________"<<endl;
	cout<<"----  Ingresa El codigo que deseas traducir ^.^ ---- "<<endl;
	cout<<"______________________________________________________"<<endl;
	cout<<"----- Para la traduccion del codigo debe colocar 'final.' al final del codigo: "<<endl<<endl;
	/*.compare es una funcion de la libreria string y nos permite comparar dos cadenas de texto.
	  nos devuelve un -1 sino se ha encontrado y un 0 si se ha encontrado.
	*/
	while(linea.compare("final.")!=0){ 
		getline(cin,linea,'\n'); //recibiendo codigo
		if(linea.compare("final.")!=0){
			traducir_codigo(linea);
			traducir_codigo("\n"); 	//Escribe un salto de linea
		}							
	}
	
	traduc_cod();
}


void funciones_archivo(string linea_cod){
		string linea_cod_aux = linea_cod;
		int flag = 0; 
		
		//apertura del archivo
		FILE* archivo = fopen(nombre_archivo,"rb");	
		if (!archivo){
			archivo = fopen(nombre_archivo,"w+b");	
		}

		int id=0;	
		
		fread(&funciones,sizeof(Funcion),1,archivo);
		
		do{
			/*las variables str_ing y str_esp toman de valor todas los nombres de las funciones en su respectivo idioma*/
			string str_ing (string(funciones.ingles)); 
			string str_esp (string(funciones.espanol)); 
			
		
			int tam = str_ing.length();  //Toma el tamaño de la palabra reservada en inglés.
			
			size_t found = linea_cod_aux.find(str_ing); //Esta funcion retorna la primer insidecia de la palabra en el string
			
			if (found!=string::npos){//Si encontro una insidencia lo reemplaza en el str original
				linea_cod_aux.replace(found,tam,str_esp); 
				flag = 1;//encontrado
			}
			
			fread(&funciones,sizeof(Funcion),1,archivo);//Lee otra linea del archivo
			id+=1;	
			
		}while(feof(archivo)==0);
		
		if(flag ==1){ 
			cout<<linea_cod_aux<<endl; //si se encontro insidencia leo el str en el cual este esta almacenado	
		}
		
		if(flag ==0){ 
			cout<<linea_cod<<endl;//si no encontro insidencia leo el str original sin cambios.
		}
		fclose(archivo);
}

//--Lectura de archivo en donde se encuentra el codigo
void traduc_cod(){
		
		string linea; 
		
		cout<<"\t\tCodigo traducido"<<endl;
		cout<<"______________________________________________________"<<endl;
		
		ifstream archivo ("cod_orginal.txt"); //Lee el archivo auxiliar que anteriormente utilizamos para recibir el codigo
		
		if (archivo.is_open()){ 
			while(getline(archivo,linea)){ // Copia linea por linea del codigo
				funciones_archivo(linea);		
			}
		    archivo.close();
		}else{
			cout << "error";	
		} 	
		
		cout<<endl;
		remove("cod_orginal.txt");//remueve el archivo auxiliar con el codigo
        	
    	cout<<endl;
		system("pause");
}

//Funcion para leer todas las funciones que existen
void leer(){
		//system(yuumi);
		FILE* archivo = fopen(nombre_archivo,"rb");	
		if (!archivo){
			archivo = fopen(nombre_archivo,"w+b");	
		}
			int id=0;	
			fread(&funciones,sizeof(Funcion),1,archivo);
			cout<<"______________________________________________________"<<endl;
			cout<<"\t ---- Listado de Palabras con su funcion ---- "<<endl;
			do{
			cout<<"______________________________________________________"<<endl;
			cout<<"Id: "<<id<<endl;
			cout<<"No. "<<funciones.n<<endl;
			cout<<"Plb. en Ingles: "<<funciones.ingles<<endl;
			cout<<"Plb. en Spanish: "<<funciones.espanol<<endl;
			cout<<"Funcion: "<<funciones.funcion<<endl;	
			fread(&funciones,sizeof(Funcion),1,archivo);
			id+=1;	
		}while(feof(archivo)==0);
		fclose(archivo);
		cout<<endl;		

		system(yone);
}

//Funcion para actualizar las funciones
void actualizar(){
	FILE* archivo = fopen(nombre_archivo,"r+b");
	int id;
	string espanol,ingles,funcion;
	cout<<"Ingrese el Id que desea Modificar: ";
	cin>>id;
	fseek(archivo,id * sizeof(Funcion),SEEK_SET);
		cout<<"Ingrese el Numero: ";
		cin>>funciones.n;
		cin.ignore();
		cout<<"Ingrese la palabra en ingles: ";
		getline(cin,ingles);
		strcpy(funciones.ingles,ingles.c_str());
		cout<<"Ingrese la traduccion en spanish: ";
		getline(cin,espanol);
		strcpy(funciones.espanol,espanol.c_str());
		cout<<"Ingrese la funcionalidad: ";
		getline(cin,funcion);
		strcpy(funciones.funcion,funcion.c_str());
		cin.ignore();
			
	fwrite(&funciones,sizeof(Funcion),1,archivo);
	fclose(archivo);
	leer();
}	

//Funcion para agregar una funcion
void agregar(){
	char continuar;
	FILE* archivo = fopen(nombre_archivo,"ab");
	string espanol,ingles,funcion;
	do{
		fflush(stdin);
		cout<<"Ingrese el Numero de palabra: ";
		cin.ignore();
		cin>>funciones.n;
		cout<<"Ingrese la palabra en ingles: ";
		cin.ignore();
		getline(cin,ingles);
		strcpy(funciones.ingles,ingles.c_str());
		cout<<"Ingrese la palabra en Spanish: ";
		cin.ignore();
		getline(cin,espanol);
		strcpy(funciones.espanol,espanol.c_str());
		cout<<"Ingrese la funcion: ";
		cin.ignore();
		getline(cin,funcion);
		strcpy(funciones.funcion,funcion.c_str());
		fwrite(&funciones,sizeof(Funcion),1,archivo);
		cout<<"Dese Agregar otra palabra (s/n): ";
		cin>>continuar;
	} while ( (continuar=='s') || (continuar=='S') );
	fclose(archivo);
}
void eliminar(){
 	
	FILE *archivo, *archivo_temp; 
	int n; 
	archivo_temp=fopen(nombre_archivo_temp,"w+b"); 
	archivo=fopen(nombre_archivo,"rb"); 
	                      				                
	cout<<"No. de la palabra a eliminar:"<<endl; 
	cin>>n;  
	                                                                            
		while(fread(&funciones,sizeof(Funcion),1,archivo)){ 
			if (funciones.n != n) 
			fwrite(&funciones,sizeof(Funcion),1,archivo_temp);  
			}
	                
		fclose(archivo_temp); 
		fclose(archivo); 
	                
		archivo_temp=fopen(nombre_archivo_temp,"rb"); 
		archivo=fopen(nombre_archivo,"wb"); 
	 
		while(fread(&funciones,sizeof(Funcion),1, archivo_temp)){ 
			fwrite(&funciones,sizeof(Funcion),1, archivo); 
		}               
	fclose(archivo_temp); 
	fclose(archivo); 
	
	leer();
}

