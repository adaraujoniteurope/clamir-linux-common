# 2025 03 03

## Control

Hoy se vuelve a probar el lazo de control legado integrado directamente en la aplicación
sin la modificación para guardar los parámetros.

Se vuelve a probar y se define un protocolo de validación de firmware para garantizar que
el control funciona adecuadamente.

Es necesario crear la aplicación de pruebas de firmware en local para no depender de la
aplicación de terceros con la finalidad de evitar que el CLAMIR dependa de un ordenador.

## Paso a Paso

1. Arreglar la bancada de pruebas. (done)

2. Hacer un manual para reproduzir el experimento.

Notas:

1. La aplicación clamir-linux-daemon en su revisión 21a7faf69f9517a1cbd70b1ea0fcbf30953b646a
tiene todo el sistema funcionando, pero no guarda los parámetros del controlador. Para eso,
es necesario cambiar la interfaz de control a lo basado en los modulos "drivers" que hice
con una interfaz uniforme.

2. Luego hace falta cambiar el controlador a la nueva versión de controlador.

3. Pero eso se supone un cambio más brusco, una vez que hace falta:

 - Autoshutter

   - Auto Shutter por Timer
   - Auto Shutter por Drift de Temperatura
   - Auto Shutter When Controller is Enabled
   - Auto Shutter Enable

 - Laser Status

   - Autodetect Laser Status
   - ON:
      Next State OFF Conditions:
         - ((int*) metadata)[1] < ((int*)mb_core_shm)[END_OF_TRACK]
   - OFF:
      Next State ON Conditions:
         - ((int*) metadata)[1] < ((int*)mb_core_shm)[START_TRACK_MOM_T]

 - System Operating Modes (Laser Operating Modes)

   - MANUAL
      laser output power is directly set by proc_var_shm[POWER_MAN] and assigned to the PWM output at mb_core_shm[PWM]
      
   - PREHEATING
      laser output power is directly set by proc_var_shm[PREHEATING_POWER] by a proc_var_shm[PREHEATING_TIME]

   - MEASUREMENT
      Modes:
       - Simple (Track is Always Counting independently if laser is on or off)
       - Complex
         - Laser Status
            If Laser is On and measurement is not sleeping

               Track Count
               Set Delay Laser On based on proc_var_shm[DELAY_LASER_ON] value


   - CONTROL

      If Delay Laser on Is Enabled
         Before Enabling Laser and Controlling It's Power Apply a Delay

      Power Width is averaged in a Circular Buffer
         Should be changed to a cumulative averaging or an exponential cumulative averaging by performance
         proc_var_shm[BUFF_SIZE] is the number of samples it takes to compute width averaging
 
 - Laser Power Output Requirements
   - Laser output power is scaled between proc_var_shm[MAX_POWER] and proc_var_shm[MIN_POWER]


4. Cambiar la integración legado a una integración
legado basado en el modulo.

4. Hacer la aplicación web del Micro CAMIR implementando
   lo que hay en el manual para reproducir allí.

5. Hay que trabajar sobre el BPC pero esto solo será
posible cuando yo pueda uniformizar el lazo de control.

6. Pués hay que resolver el problema de los framios
perdidos. Crear una aplicación en que se pueda contar
cuando el id del frame sea distinto en una secuencia
crescente.

7. Quitar el LabJack para integración directa con el
MicroCAMIR.

Notificación de frame perdido en image_writer.

## Estructura

Adaptar la estructura de lambdatech para el CLAMIR arquitecturalmente, utilizar los signals
y slots... pero tengo que estudiar un poco más como voy a construir esto con clareza antes
de implementarlo.