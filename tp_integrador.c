#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CANTIDAD_USUARIOS 10
#define MAX_INTENTOS 3

// Estructura usuario
struct usuario
{
  int numeroDeCuenta;
  char contrasena[15];
  char nombre[30];
  float saldo;
  bool estado;
};

// Lista de usuarios
struct usuario usuarios[CANTIDAD_USUARIOS];

// Carga los datos de los usuarios. Número de cuentas: 100[i] y contraseñas: contrasena[i], con i [0,9].
// Ejemplo: Número de cuenta: 1000, contraseña: contrasena0.
void cargar_usuarios()
{
  for (int i = 0; i < CANTIDAD_USUARIOS; i++)
  {
    usuarios[i].numeroDeCuenta = 1000 + i;
    sprintf(usuarios[i].contrasena + strlen(usuarios[i].contrasena), "contrasena%d", i);
    sprintf(usuarios[i].nombre + strlen(usuarios[i].nombre), "usuario%d", i);
    usuarios[i].saldo = (float)(rand() % 10000);
    usuarios[i].estado = rand() % 2 == 0;
  }
}

// Limpia el buffer.
void limpiar_buffer()
{
  while (getchar() != '\n')
    ;
}

// Valida que el número de cuenta sea de una de las 10 cuentas creadas, si lo es, procede a solicitar
// la contraseña de la misma con un máximo de 3 intentos.
int validar_cuenta(int numeroDeCuenta)
{
  int i = 0;
  while (i < CANTIDAD_USUARIOS)
  {
    if (usuarios[i].numeroDeCuenta == numeroDeCuenta)
    {
      if (!usuarios[i].estado)
      {
        printf("Su cuenta se encuentra bloqueada, comuníquese con la entidad bancaria para su restablecimiento.\n");
        printf("-----------------------------------------\n");
        return -1;
      }
      else
      {
        int intentos = 0;
        while (intentos < MAX_INTENTOS)
        {
          char contrasenaIngresada[15];
          printf("Ingrese su contraseña: ");
          scanf("%s", contrasenaIngresada);

          if (strcmp(contrasenaIngresada, usuarios[i].contrasena) == 0)
          {
            printf("-----------------------------------------\n");
            printf("Bienvenido %s!\n", usuarios[i].nombre);
            return i;
          }
          else
          {
            printf("Contraseña incorrecta, %d intentos restantes.\n", MAX_INTENTOS - (intentos + 1));
            printf("-----------------------------------------\n");
            intentos++;
          }
        }
        printf("No se permiten más intentos. Su cuenta ha sido bloqueada, comuníquese con la entidad bancaria para su restablecimiento.\n");
        printf("-----------------------------------------\n");
        usuarios[i].estado = false;
        return -1;
      }
    }
    i++;
  }
  printf("No existe una cuenta con ese número.\n");
  printf("-----------------------------------------\n");
  return -1;
}

// Solicita un número de cuenta y verifica que el mismo sea un entero.
int iniciar_sesion()
{
  int numeroDeCuentaIngresado;
  printf("Ingrese su número de cuenta: ");
  if (scanf("%d", &numeroDeCuentaIngresado) == 1)
  {
    return validar_cuenta(numeroDeCuentaIngresado);
  }
  else
  {
    printf("Número de cuenta inválido.\n");
    printf("-----------------------------------------\n");
    limpiar_buffer();
    return -1;
  }
}

// Deposita un monto de dinero en la cuenta del usuario.
void depositar(int usuario)
{
  bool bandera = true;
  float monto;
  do
  {
    printf("Ingrese el monto de dinero a depositar: ");
    if (scanf("%f", &monto) == 1 && monto > 0)
    {
      usuarios[usuario].saldo += monto;
      printf("Deposito realizado con éxito.\n");
      bandera = false;
    }
    else
    {
      printf("Monto inválido.\n");
      limpiar_buffer();
    }
  } while (bandera);
}

// Extrae un monto de dinero en la cuenta del usuario.
void extraer(int usuario)
{
  bool bandera = true;
  float monto;
  do
  {
    printf("Ingrese el monto de dinero a extraer: ");
    if (scanf("%f", &monto) == 1 && monto <= usuarios[usuario].saldo && monto >= 0)
    {
      usuarios[usuario].saldo -= monto;
      printf("Extracción realizada con éxito.\n");
      bandera = false;
    }
    else
    {
      printf("Monto inválido.\n");
      limpiar_buffer();
    }
  } while (bandera);
}

// Muestra el menú de acciones del cajero y solicita una opción.
void menu(int usuario)
{
  int opcion, operaciones = 0;
  bool salir = false;

  do
  {
    printf("-----------------------------------------\n");
    printf("1) Deposito.\n2) Extracción.\n3) Consultar Saldo.\n4) Mostrar la cantidad de operaciones realizadas y el saldo actual.\n5) Cerrar sesión\n");
    printf("-----------------------------------------\n");
    printf("Seleccione una opción: ");
    if (scanf("%d", &opcion) == 1)
    {
      switch (opcion)
      {
      case 1:
        depositar(usuario);
        operaciones++;
        break;
      case 2:
        extraer(usuario);
        operaciones++;
        break;
      case 3:
        printf("Su saldo es de $%.2f.\n", usuarios[usuario].saldo);
        operaciones++;
        break;
      case 4:
        printf("Lleva %d operaciones realizadas y su saldo es de $%.2f.\n", operaciones, usuarios[usuario].saldo);
        break;
      case 5:
        salir = true;
        printf("Hasta pronto!\n\n\n\n");
        break;
      default:
        printf("No existe esa opción.\n");
        break;
      }
    }
    else
    {
      printf("Opción inválida.\n");
      limpiar_buffer();
    }
  } while (!salir && operaciones < 10);

  if (operaciones == 10)
  {
    printf("-----------------------------------------\n");
    printf("Llegó al límite de operaciones. Fin. Gracias!\n");
  }
}

// Inicia el programa preguntando si desea iniciar sesión o terminar.
void iniciar_sistema()
{
  int usuario = -1;

  cargar_usuarios();

  do
  {
    printf("-----------------------------------------\n");
    printf("Bienvenido al sistema del banco.\n");
    printf("-----------------------------------------\n");
    do
    {
      usuario = iniciar_sesion();
    } while (usuario < 0);

    menu(usuario);
  } while (true);
}

int main()
{
  iniciar_sistema();
  return 0;
}