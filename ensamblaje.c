#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
//#include <regex.h>


#define Max_Int 10 // Número máximo de caracteres que puede tener un valor int
#define Max_Art_Vwndidos 4 // Número máximo de caracteres que puede tener un valor de vendidos de un articulo
#define Max_Art_Nombre 21 // Número máximo de caracteres que puede tener un nombre de articulo
#define Max_articulos 10 // Número máximo de articulos por factura
//Para las vocales acentuadas y la ñ
enum{
    ü = 129,
    é = 130,
    É = 144,
    ö = 148,
    Ö = 153,
    Ü = 154,
    á = 160,
    í = 161,
    ó = 162,
    ú = 163,
    ñ = 164,
    Ñ = 165,
    Á = 181,
    Ó = 224,
    Í = 214,
    Ú = 233
};


typedef struct Articulo
{
    int codigo;
    char nombre[Max_Art_Nombre];
    int vendidos;
    int costo_u;
    long long subtotal;
} Articulo;

typedef struct Factura
{
    int id;
    int num_art;
    long long total;
    struct Articulo detalle[Max_articulos];

} Factura;

typedef struct Usuario
{
    char username[Max_Art_Nombre];
    char userpassword[Max_Art_Nombre];
    int usertype;
} Usuario;

bool leerFacturas();
int cast_int(char valor[Max_Int]);
bool val_str(char valor[Max_Int]);
bool val_articulo(Articulo *art, char *codigo, char *nombre, char *vendidos, char *costo_u);
bool tiene_articulo(Articulo art);
bool val_factura(Factura *fact);
bool sg_articulo(FILE* archivo, Articulo art, Factura fact);
bool cout_articulo(FILE* archivo, Articulo art, int fact_id);
bool sg_factura(Factura fact);
bool bu_articulo();
bool bu_factura(int id);
bool car_facturas();
void limpiarStdin();
void sis_art();
bool bu_usuario(Usuario *usuario, char *username, char *password);
void Pantalla_log(char *username, char *password);
bool letra_hisp (unsigned x);
bool esp_caracter (unsigned x);
void gotoxy(int x, int y);
void Rect(int xs, int ys, int xi, int yi);


int main(){

    char username[Max_Art_Nombre], password[Max_Art_Nombre];

    Pantalla_log(username, password);



    return 0;
}

void Pantalla_log(char *username, char *password){
    system("mode con: cols=80 lines=25");
    system("COLOR 70");
    recuadro(2,0,78,24);
    int i, rep;

    gotoxy(20,9);
    printf("Ingrese su Usuario:");
    while (rep = getch())
    {
        gotoxy(48, 11); printf("                              ");
        if (rep == 13){
            username[i] = '\0';
            break;
        } else if ( rep == 8){
            if ( i > 0 ){
                i--;
                gotoxy(23 + i,11); printf(" \b");
            }
        } else {
            if (!(isalnum(rep) || letra_hisp(rep))){
                gotoxy(48, 11); printf("El \"%c\" es un caracter inv%clido", rep, á);
            }

            else{
                if ( i < 20){
                    gotoxy(23 + i,11);
                    printf("%c", rep);
                    username[i] = rep;
                    i++;
                }else {
                    gotoxy(48, 11); printf("Alcanz%c el m%cximo de carateres", ó, á);
                }
            }
        }
    }
    gotoxy(20,13);printf("Contrase%ca:", ñ);
    i = 0; //Reseteo la variable para el ciclo while
    while (rep = getch())
    {
        gotoxy(48, 15); printf("                              ");
        if (rep == 13){
            password[i] = '\0';
            break;
        } else if ( rep == 8){
            if ( i > 0 ){
                i--;
                gotoxy(23 + i,15); printf(" \b");
            }
        } else {
            if (!(isalnum(rep) || letra_hisp(rep) || esp_caracter(rep))){
                gotoxy(48, 15); printf("El \"%c\" es un caracter inv%clido", rep, á);
            }else{
                if ( i < 20){
                    gotoxy(23 + i,15);
                    printf("*");
                    password[i] = rep;
                    i++;
                }
            }
        }
    }
    system("cls");
}

bool leerFacturas()  {
    FILE* archivo;

    archivo = fopen("facturas.txt", "r");

    if (archivo == NULL) {
        printf("Aún no hay facturas para leer");
        return false;
    }

    return true;
}

int cast_int(char valor[Max_Int]){
    int i;
    for ( i = 0; valor[i] != '\0'; i++)
    {
        if ((!isdigit(valor[i]))){

            printf("no has introducido una entrada válida\n");
            return 0;
        }
    }
    return atoi(valor);

}

bool val_str(char valor[Max_Int]){
    int i;
    for ( i = 0; valor[i] != '\0'; i++)
    {
        if (!(isalnum(valor[i]))){
            //|| isspace(valor[i]) || (valor[i]>=160 && valor[i]<= 165) || valor[i] == 130 || valor[i] == 144 || valor[i] == 181 || valor[i] == 214 || valor[i] == 224 || valor[i] == 233
            printf(" %c o %d no es una entrada válida\n", valor[i], valor[i]);
            return false;
        }
    }
    return true;

}

bool val_articulo(Articulo *art,
                         char *codigo,
                         char *nombre,
                         char *vendidos,
                         char *costo_u) {
    strncpy(art->nombre, nombre, Max_Art_Nombre - 1);

    if (!val_str(art->nombre)){
        printf("Fallo en el Nombre.\n");
        return false;
    }

    art->codigo = cast_int(codigo);
    art->costo_u = cast_int(costo_u);
    art->vendidos = cast_int(vendidos);
    if (!(art->codigo>0)){
        printf("Fallo en el código del articulo. %d : \n", art->codigo);
        return false;
    }
    if (!(art->vendidos>0)){
        printf("Fallo en los vendidos del artículo.\n");
        return false;
    }
    if (!(art->costo_u>0)){
        printf("Fallo en el costo unitario.\n");
        return false;
    }

    art->subtotal = (long long)art->costo_u * art->vendidos;
    return true;
}

bool tiene_articulo(Articulo art){
    return art.vendidos>0;
}

//prerequisito todos los articulos deben ser validos
bool val_factura(Factura *fact){
    //TODO: Hacer las vaidaciones de facturas

    fact->num_art = 0;
    fact->total = 0;

    for(int i = 0; i<Max_articulos; i++){
        if (tiene_articulo(fact->detalle[i]))
        {
            fact->num_art += fact->detalle[i].vendidos;
            fact->total += fact->detalle[i].subtotal;
        }
        else
            break;
    }
    return (fact->num_art)>1;
}

bool sg_articulo(FILE* archivo, Articulo art, Factura fact) {
    if (fprintf(archivo, "%d|%d|%s|%d|%d|%lld\n", fact.id, art.codigo, art.nombre, art.vendidos, art.costo_u, art.subtotal)>1)
        return true;

    return false;
}

bool cout_articulo(FILE* archivo, Articulo art, int fact_id) {
    if (fprintf(archivo, "%d|%d|%s|%d|%d|%lld\n", fact_id, art.codigo, art.nombre, art.vendidos, art.costo_u, art.subtotal)>1)
        return true;

    return false;
}

bool sg_factura(Factura fact){
    bool ok = true;

    if (!val_factura(&fact))
        return false;

    //TODO: escribir una copia del archivo y una vez que la modificación es validada se reemplaza los archivos originales

    FILE* rcfacturas;
    FILE* rcdetalle;
    if ((rcfacturas = fopen("facturas.txt", "a")) == NULL)
    {
        return false;
    }
    if ((rcdetalle = fopen("detalle.txt", "a")) == NULL)
    {
        fclose(rcfacturas);
        return false;
    }

    if (!fprintf(rcfacturas, "%d|%d|%lld\n", fact.id, fact.num_art, fact.total)>1)
        ok = false;

    for(int i = 0; i<Max_articulos; i++){
        if (tiene_articulo(fact.detalle[i]))
        {
            if (!sg_articulo(rcdetalle, fact.detalle[i], fact))
                ok = false;
        }
        else
            break;
    }

    fclose(rcdetalle);
    fclose(rcfacturas);

    return ok;
}

bool bu_articulo(){
    FILE *archivo;
    if ((archivo = fopen("detalle.txt", "r")) == NULL)
    {
        printf("Error al abrir los ficheros, puede que no existan.\n");
        return false;
    }
    else {
        int fact_id;
        Articulo art;

        while (!feof(archivo))
        {
            if (fscanf(archivo, "%d|%d|%[^|]|%d|%d|%lld\n", &fact_id, &art.codigo, art.nombre, &art.vendidos, &art.costo_u, &art.subtotal) == 6)
            {
                if (art.subtotal > 120000)
                {
                    printf("Num. Factura: %d\n", fact_id);
                    printf("Cód. artículo: %d\n", art.codigo);
                    printf("Nombre: %s\n", art.nombre);
                    printf("Se vendieron: %d\n", art.vendidos);
                    printf("Costo U: %d     Subtotal: %lld\n", art.costo_u, art.subtotal);
                    printf("--------------------------------------------------------------------------\n");
                }
                else {}
            }
            else { fclose(archivo); return false;}
        }
        fclose(archivo);
        return true;
    }
}

bool bu_factura(int id){
    if (id < 0){

        printf("Ha introducido un identificador inválido.\n");
        return false;
    }
    FILE *archivo;
    if ((archivo = fopen("detalle.txt", "r")) == NULL)
    {
        printf("Error al abrir los ficheros, puede que no existan.\n");
        return false;
    }
    else {
        int fact_id, primero = 0;
        long long total = 0;
        Articulo art;

        while (!feof(archivo))
        {

            if (fscanf(archivo, "%d|%d|%[^|]|%d|%d|%lld\n", &fact_id, &art.codigo, art.nombre, &art.vendidos, &art.costo_u, &art.subtotal) == 6)
            {

                if ((fact_id -1) == id){
                    printf("TOTAL:      %lld\n", total);
                    printf("------------------------------------------------------------------------\n");
                    break;
                }

                if (fact_id == id)
                {
                    if ( primero == 0)
                    {
                        //TODO: hacer una impresión específica para facturas, que no se repita el num de factura
                        printf("        Num. Factura: %d\n", fact_id);
                        printf("Cód. artículo: %d\n", art.codigo);
                        printf("%s ... %d\n", art.nombre, art.vendidos);
                        printf("    Costo U: %d     Subtotal: %lld\n", art.costo_u, art.subtotal);
                        total += art.subtotal;
                        primero = 1;
                    }
                    else {
                        printf("Cód. artículo: %d\n", art.codigo);
                        printf("%s ... %d\n", art.nombre, art.vendidos);
                        printf("    Costo U: %d     Subtotal: %lld\n", art.costo_u, art.subtotal);
                        total += art.subtotal;
                    }
                }
                else {}
            }
            else { fclose(archivo); return false;}
        }
        fclose(archivo);
        if (id > fact_id){

            printf("El identificador no se encuentra asignado a ninguna factura del sistema.\n");
            return false;
        }
        return true;
    }
}

bool car_facturas(){
    FILE *archivo;
    if ((archivo = fopen("facturas.txt", "r")) == NULL)
    {
        printf("Error al abrir los ficheros, puede que no existan.\n");
        return false;
    }
    else {
        int fact_id, num_art, num_factas = 0, primero = 0;
        long long total_fact;

        while (!feof(archivo))
        {

            if (fscanf(archivo, "%d|%d|%lld\n", &fact_id, &num_art, &total_fact) == 3)
            {
                    if ( primero == 0)
                    {
                        printf("Fact. ID|Num. Art.|Total Fact.\n");
                        printf("   %d   |    %d   |    %lld\n", fact_id, num_art, total_fact);
                        num_factas += 1;
                        primero = 1;
                    }
                    else {
                        printf("   %d   |    %d   |    %lld\n", fact_id, num_art, total_fact);
                        num_factas += 1;
                    }
            }
            else { fclose(archivo); return false;}
        }
        fclose(archivo);
        printf("\nTOTAL DE FACTURAS REGISTRADAS     ..........    %d.\n", num_factas);
        printf("----------------------------------------------------------------------------\n");
        return true;
    }
}

void limpiarStdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Consumir caracteres restantes
    }
}

/*TODO: al convertirlo en funcion ahora necesita de dos enter para la toma de datos, hay que solucionarlo,
  también hay que hacer una salida si hay un articulo invalido*/
void sis_art(){
    Factura fact = { 1, 0, 0};
    Articulo art;
    int i = 0, p = 1, numItems = 0;
    char codigo[Max_Int], nombre[Max_Art_Nombre], vendidos[Max_Art_Vwndidos], costo_u[Max_Int];

    do
    {
        //TODO: VAlidar todas estas entradas, espacios en blanco y entradas vácias
        printf("Introduzca el código del artículo que desea añadir a la factura.\n");
        fgets(codigo, sizeof(codigo), stdin);
        limpiarStdin();
        printf("Introduzca el nombre del artículo que desea añadir a la factura.\n");
        fgets(nombre, sizeof(nombre), stdin);
        limpiarStdin();
        printf("Introduzca costo que tiene este artículo.\n");
        fgets(costo_u, sizeof(costo_u), stdin);
        limpiarStdin();
        printf("¡Cuántos %s va ha comprar?\n", nombre);
        fgets(vendidos, sizeof(vendidos), stdin);
        limpiarStdin();
        if (val_articulo(&art, codigo, nombre, vendidos, costo_u))
        {
            fact.detalle[i] = art;
            i++;
            //TODO: Hacer validación de esta entrada
            printf("Desea:\n1.Añadir otro art.  2. Cerrar Factura.  3.Cancelar registro\n");
            scanf("%d", &p);
            switch (p)
            {
            case 1:
                printf("Añada el siguiente articulo.\n");
                continue;

            case 2:
                if (sg_factura(fact)){
                    printf("Se a guardado correctamente.\n");
                    break;
                }
                else {
                    printf("Ha ocurrido un erro.");
                    continue;
                    }

            case 3:
                printf("Se cance2ó el registro.\n");
                break;

            default:
                break;
            }
        }
        else{
            printf("Ha introducido un artículo inválido.\n");
        }

        printf("Valor p: %d\n", p);

    } while (p == 1);


        /*else{
            printf("Ha introducido una entrada invalida.\n");
            printf("Desea cancelar el registro.\n");
            printf("1. Para registrar un otro artículo.     2. Para cancelar el registro de la factura.");
            //TODO: Validar esta entrada
            scanf("%d", p);
            if (p == 1)
                {}
            else
                break;
        }*/
}

//TODO: Hacer que el programa haga su propio contador de facturas

bool bu_usuario(Usuario *usuario, char *username, char *password){
    FILE *archivo;
    if ((archivo = fopen("usuarios.txt", "r")) == NULL)
    {
        printf("Error al abrir los ficheros, puede que no existan.\n");
        return false;
    }
    else {
        int fact_id;
        Articulo art;
        while (!feof(archivo))
        {
            if (fscanf(archivo, "%[^|]|%[^|]|%d\n", usuario->username, usuario->userpassword, &usuario->usertype) == 3)
            {
                if ((strcmp(usuario->username, username) == 0) && (strcmp(usuario->userpassword, password) == 0))
                {
                    return true;
                }
                else{}
            }
            else { fclose(archivo); return false;}
        }
        fclose(archivo);
        return false;
    }
}

bool letra_hisp (unsigned x){
     if ( x>= 160 && x <= 165)
        return true;
     if ( x == ü || x == é || x == É || x == ö || x == Ö || x == Ü || x == Á || x == Í || x == Ó || x == Ú)
        return true;
     else
        return false;
}

bool esp_caracter (unsigned x){
     if ( x>= 33 && x <= 47)
        return true;
     else
        return false;
}

void gotoxy(int x, int y){
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon,dwPos);
}

void recuadro(int xs, int ys, int xi, int yi){
    int i;
    for(i = xs; i <= xi; i++){
            if (i == xs){
                gotoxy(i, ys); printf("%c", 201);
                gotoxy(i, yi); printf("%c", 200);
            }else if (i == xi){
                gotoxy(i, ys); printf("%c", 187);
                gotoxy(i, yi); printf("%c", 188);
            }else{
                gotoxy(i, ys); printf("%c", 205);
                gotoxy(i, yi); printf("%c", 205);
            }
    }
    for(i = ys + 1; i < yi; i++){
            gotoxy(xs, i); printf("%c", 186);
            gotoxy(xi, i); printf("%c", 186);
    }
}
