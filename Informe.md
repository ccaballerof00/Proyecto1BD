---


---

<h1 id="proyecto-1-organización-de-archivos-y-control-de-concurrencia">Proyecto 1: Organización de Archivos y Control de Concurrencia</h1>
<p><strong>Curso</strong>: Base de datos II<br>
<strong>Fecha</strong>: 12 de octubre del 2020<br>
<strong>Integrantes</strong>:</p>
<ul>
<li>Caballero Falcón, Cristian Andrés (201810431)</li>
<li>Figueroa Gómez, Nicolas Jorge (201810294)</li>
</ul>
<h2 id="índice">Índice</h2>
<ol>
<li><strong>Introducción</strong><br>
1.1 Objetivo<br>
1.2 Descripción de datos<br>
1.3 Resultados esperados</li>
<li><strong>Técnicas</strong><br>
2.1 Sequential File<br>
2.2 Extendible Hashing<br>
2.3 Transacciones concurrentes</li>
<li><strong>Resultados experimentales</strong><br>
3.1 Comparativa entre ambos métodos<br>
3.2 Discusión y análisis de resultados</li>
<li><strong>Conclusiones</strong></li>
</ol>
<h2 id="introducción">1. Introducción</h2>
<p>En este informe presentaremos dos técnicas de almacenamiento de la información que hemos aprendido en las primeras semanas del curso: Sequential File y Extendible Hashing. Explicaremos la lógica que aplicamos, los métodos que hemos creado y finalmente haremos un análisis de rendimiento entre ambas técnicas para un mismo conjunto de datos.</p>
<p><strong>1.1 Objetivos</strong><br>
El objetivo de nuestro informe es dar a conocer la información que hemos podido recopilar durante la implementación y testeo de las dos técnicas de almacenamiento de datos anteriormente mencionadas. Asimismo buscamos hacer una comparación entre ambas técnicas para poder evaluar cuál es más eficiente y bajo qué circunstancias.</p>
<p><strong>1.2 Descripción de datos</strong><br>
Los datos fueron obtenidos a través del siguiente <a href="https://www.generatedata.com/">generador</a>` Allí generamos un archivo csv con 10000 registros y para nuestros testeos estaremos usando variantes de distintos tamaños de ese archivo. Nuestros registros tienen la siguiente estructura:</p>

<table>
<thead>
<tr>
<th>Variable</th>
<th>Tipo</th>
<th>Tamaño(bytes)</th>
</tr>
</thead>
<tbody>
<tr>
<td>Id</td>
<td>int</td>
<td>4</td>
</tr>
<tr>
<td>Nombre</td>
<td>char[]</td>
<td>15</td>
</tr>
<tr>
<td>Apellido</td>
<td>char[]</td>
<td>15</td>
</tr>
<tr>
<td>Correo</td>
<td>char[]</td>
<td>30</td>
</tr>
<tr>
<td>Ciudad</td>
<td>char[]</td>
<td>20</td>
</tr>
<tr>
<td>Pais</td>
<td>char[]</td>
<td>40</td>
</tr>
<tr>
<td>CodigoPais</td>
<td>char[]</td>
<td>2</td>
</tr>
</tbody>
</table><p>Estos registros representan a los empleados de una corporación en específico.<br>
La estructura de cada registro recibirá pequeñas adiciones de punteros en algunos casos para permitir la correcta implementación de las técnicas de organización de archivos.</p>
<p><strong>1.3 Resultados esperados</strong><br>
Los costos teóricos para un Sequential File son:</p>
<ul>
<li>Inserción:</li>
<li>Búsqueda:</li>
</ul>
<p>Y los costos teóricos para un Extendible Hashing son:</p>
<ul>
<li>Inserción:</li>
<li>Búsqueda:</li>
</ul>
<p>Como se puede ver, teóricamente el Extendible Hashing es más eficiente, por lo tanto esperamos que esta diferencia entre ambas técnicas se vea claramente evidenciada en el testeo de las implementaciones.</p>
<h2 id="técnicas">2. Técnicas</h2>
<p><strong>2.1 Sequential File</strong><br>
El método Sequential File consta de 2 archivos en los cuales se estará almacenando la información, uno principal (donde se guardará data ordenada) y uno auxiliar (donde se guardarán los inserts que se hagan a la base de datos).<br>
Consideraciones a tener en cuenta:</p>
<ul>
<li>Los registros tienen 2 variables adicionales en su estructura, ambas son usadas para referenciar dónde (archivo y posición) se encuentra el siguiente registro.</li>
<li>La reconstrucción del archivo principal se hace cada 10 inserciones, como se podrá observar en el código del repositorio.</li>
</ul>
<p>Métodos:</p>
<ul>
<li>Búsqueda: Para la búsqueda se realiza una búsqueda binaria en el archivo principal, si se encuentra ahí el registro con el Id que se busca, se retorna. Sino se toma como referencia el registro con el Id menor más cercano al Id que se busca y se revisa si es que este registro “anterior” tiene un siguiente en el archivo auxiliar. De ser así, se continúa la búsqueda en el archivo auxiliar hasta encontrar el registro pedido o hasta encontrar un registro cuyo Id sea mayor al que se busca, ahí se rompe la búsqueda y se devuelve un error. Cabe destacar que las iteraciones a lo largo del archivo aux se hacen usando los punteros que se guardan en los registros. Otra cosa importante es que en nuestra implementación de la búsqueda hicimos que, además de retornar el registro pedido, se retorne la posición del registro “anterior” al pedido. Esto lo hicimos para poder tomar este “anterior” y utilizarlo para la inserción y eliminación.</li>
<li>Inserción: Para insertar un registro en lo que se hace es, realizar una búsqueda para ver en dónde se debe colocar este registro. Esta búsqueda retornará el registro con el Id menor al del registro que se quiere insertar. Primero se revisa si es que el registro a insertar puede ser colocado directamente en el archivo principal de datos, aprovechando que puede haber un registro eliminado que no ha sido suplido aún. Si no se da este caso, lo que se hace es realizar la inserción en el archivo auxiliar, primero se revisa con el entero de freelist si es que hay una posición eliminada en donde insertar el registro. Si la hay, se inserta ahí y se actualizan los punteros para seguir manteniendo el orden a lo largo de los 2 archivos. Si no la hay, se inserta el nuevo registro al final del archivo auxiliar y se actualizan los punteros igualmente.</li>
<li>Eliminación: Para la eliminación lo que hacemos es, realizar la búsqueda del registro y una vez que ya lo obtuvimos, cambiar su puntero next al valor -5 y finalmente actualizar los valores de punteros de su registro anterior para que apunte a su siguiente. En el caso del aux, además de eliminar el registro, se debe actualizar la freelist. Otra cosa especial en el archivo auxiliar es que un archivo eliminado puede no necesariamente tener -5 como next, ya que este valor puede ser cambiado para poder tener una cadena de los registros que han sido eliminados, empezando desde el freelist. Esto no es un problema ya que, si bien no tiene la referencia del -5 para saber que está eliminado, si están referenciados anteriormente por la freelist, por lo tanto se sabe que está eliminado.  Otro detalle importante es que cuando se elimina un registro del archivo principal, lo que se hace es revisar si es que este registro tiene un anterior o siguiente en el archivo auxiliar, de ser así, el registro del archivo auxiliar pasa a reemplazar al que se quiere eliminar en el archivo principal.</li>
</ul>
<p><strong>2.2 Extendible Hashing</strong><br>
El extendible Hashing es un tipo de método de hash dinámico. Este está compuesto de directorios y contenedores o “buckets”. Es un método que tiene como característica principal ser muy flexible, además que la función hash también experimenta cambios dinámicos durante su proceso.</p>
<p>Para esto se debe determinar las características específicas que debe tener cada uno durante su proceso.</p>
<p>En primer lugar, la cantidad de directorios es igual a dos elevado a la profundidad global. Cabe recalcar que no todos los directorios siempre apuntan al mismo contenedor o bucket. Además, que estos no siempre almacenarán la misma dirección de cada contenedor, porque pueden cambiar cada vez que se realiza una expansión.</p>
<p>En segundo lugar, tenemos a los buckets o contenedores que almacenan las claves hash. Estos, son apuntados por los directorios. Cada contenedor puede tener más de un puntero si su profundidad local es menor que la profundidad global.</p>
<p>Se debe tener en cuenta que si el tamaño de cada contenedor supera su factor de bloque, este debe dividirse porque un contenedor overflow. Y si, su profundidad local llega a ser mayor que la profundidad global produce que la profundidad global aumente, y que el directorio se reconstruya.</p>
<p>Datos importantes de la estructura</p>
<ul>
<li>
<p>El directorio tiene m entradas donde m = 2 elevado a la profundidad global.</p>
</li>
<li>
<p>Cada entrada del directorio puede o no tener asociado un contenedor.</p>
</li>
<li>
<p>Cada bucket o contenedor puede tener o no otro bucket overflow asociado.</p>
</li>
<li>
<p>La función hash que se ha usado consiste en usar la key mod (el número de buckets) y luego convertir de decimal a binario.</p>
</li>
</ul>
<p>Métodos:</p>
<ul>
<li>
<p>Búsqueda: Para la búsqueda, se le pasa como parámetro de la función una key que pasara por una función hash. Luego de ello, buscaremos en el directorio el puntero al bucket el cual es igual a la key pasada. Después, se retorna todos los registros que contenía dicho bucket o contenedor. Finalmente, se realizará una búsqueda secuencial al registro al que queremos acceder.</p>
</li>
<li>
<p>Inserción: Para la inserción, se le pasa como parámetro de la función una key y el registro que queremos insertar. Primero, como ocurre con la búsqueda, el parámetro key pasará por una función hash. Esta, retorna un valor en el directorio en donde lo direccionará al bucket o contenedor correspondiente. Una vez ahí, se abrirá el bucket o contenedor y se escribirá al final el registro al que queremos ingresar. Debemos tener en cuenta que si existe un overflow, deberemos dividir el contendor y actualizar los punteros de estos con sus respectivas direcciones dentro del índice. Asimismo, si la profundidad local es mayor que la profundidad local, se debería realizar una reconstrucción del directorio actualizando todos sus punteros a los buckets o contenedores.</p>
</li>
<li>
<p>Eliminación: Para la eliminación, se le pasa como parámetro la key que pasara por medio de la función hash así como sucede en la búsqueda. Después, nos dirigimos al bucket o contenedor, realizamos una copia del mismo menos el registro que deseamos eliminar. Finalmente, debemos actualizar los punteros, además el directorio si este necesita una reconstrucción por eliminación de algún registro.</p>
</li>
</ul>
<p><strong>2.3 Transacciones concurrentes</strong><br>
Para poder simular este proceso, usamos la estructura Sequential File, además de 3 threads que cumplen el trabajo de cada transacción.</p>
<p>Por otro lado, se modificó la clase Sequential File implementando tres funciones para usarla fácilmente en la implementación de threads. Como se puede observar en las siguientes líneas.</p>
<p>La función FuncionHilosAdd, está creará un objeto tipo SequetialFile y luego llamará a la función add.</p>
<pre><code>void FuncionHilosAdd(Empleado empleado){
auto test = SequentialFile("Data2.csv");
test.ConvertirCsvaDat&lt;Empleado&gt;();
test.load&lt;Empleado&gt;();
test.loadaux&lt;Empleado&gt;();
test.add(empleado);
}
</code></pre>
<p>La función FuncionHilosSearch, está creará un objeto tipo SequetialFile y luego llamará a la función search.</p>
<pre><code>void FuncionHilosSearch(int id){
auto test = SequentialFile("Data2.csv");
test.ConvertirCsvaDat&lt;Empleado&gt;();
auto t = test.Search&lt;Empleado&gt;(id,i,c);
if(t.Id == -404)
{
cout&lt;&lt;"No se encontro ninguna coincidencia para tu busqueda"&lt;&lt;endl;
}
else
{
t.print();
}
}
</code></pre>
<p>La función FuncionHiloDelete, está creará un objeto tipo SequentialFile y luego llamará a la función delete.</p>
<p>void FuncionHilosDelete(int ide){</p>
<p>cout &lt;&lt; “Se elimino correctamente” &lt;&lt; endl;</p>
<p>}</p>
<p>El objetivo de estas transacciones es simular mediante hilos como estos pueden acceder a las diferentes funciones, de algunos casos respectivos. Como por ejemplo, el thread01 ejecutará la add. Luego, thread02 ejecutará la función search y finalmente el thread03 ejecutará la función delete. Para esto, se establecieron los join() específicos para que cuando se llamen, no sucedan casos como una lectura sucia, al momento de que el thread02 pueda buscar una llave antes de que el thread 03 la pueda eliminar y asi el thread02 no lea una llave eliminada por thread03.</p>
<p>std::thread thread01(&amp;FuncionHilosAdd, t1);</p>
<p>thread01.join();</p>
<p>int ide;</p>
<p>cout&lt;&lt;“Ingrese el id que desea eliminar”&lt;&lt;endl;</p>
<p>cin&gt;&gt;ide;</p>
<p>std::thread thread02(&amp;FuncionHilosSearch, ide);</p>
<p>thread02.join();</p>
<p>int ideDel;</p>
<p>std::thread thread03(&amp;FuncionHilosDelete, ideDel);</p>
<p>thread03.join();</p>
<h2 id="resultados-experimentales">3. Resultados experimentales</h2>
<p>Procesador: Intel® Core™ i7-8750H CPU @ 2.20GHz</p>
<p><strong>3.1 Comparativa entre ambos métodos</strong><br>
En este caso, solo pudimos hacer las pruebas con el sequential file obteniendo los siguientes resultados:</p>
<ul>
<li>Búsqueda: Probando con distintos Ids la búsqueda con el csv de 10000 datos, se retorna un tiempo de ejecución menor a 10^(-5) segundos.</li>
<li>Inserción: Probando con distintos Ids la inserción con el csv de 10000 datos, se retorna un tiempo de ejecución menor a 10^(-5) segundos.</li>
</ul>
<p><strong>3.2 Discusión y análisis de resultados</strong><br>
Como se puede ver, la búsqueda e inserción retorna tiempos de ejecución muy cortos debido a que ambos se ejecutan en un O(logn), en el caso de la inserción es O(logn + k), solo que al tener nosotros como límite de tamaño del archivo auxiliar 10, ese k puede llegar a ser despreciable convirtiéndose en un O(logn).</p>
<h2 id="conclusiones">Conclusiones</h2>
<ul>
<li>Como pudimos apreciar en la etapa de testeo, las operaciones en el Sequential File son muy rápidas debido a que se trabajan aproximadamente en O(logn). Sin embargo, la parte más costosa de esta técnica es a la hora de hacer la reconstrucción del archivo, ya que se hace en O(n) lo cual para largas cantidades de datos puede ser mucho tiempo de ejecución.</li>
<li>La idea del Extendible Hashing teóricamente es más eficiente, sin embargo, como pudimos aprender durante el desarollo del proyecto, es bastante más difícil de implementar, por lo que no pudimos llegar a hacer testeos que corroboren si es que existía o no una diferencia significativa entre el Sequential File y el Extendible Hashing.</li>
</ul>
<h2 id="presentación">Presentación</h2>
<p>El ejecutable de nuestro proyecto se encuentra incluido en el repositorio bajo el nombre de ChunteSQL.exe, además también allí podrá encontrar nuestro código y los videos explicativos de cada implementación.</p>
<p>SeqFile:<br>
<a href="https://drive.google.com/file/d/1Ek7aVIIyzQgwkHuDtVmTwIeEPrpX8u2n/view?usp=sharing">https://drive.google.com/file/d/1Ek7aVIIyzQgwkHuDtVmTwIeEPrpX8u2n/view?usp=sharing</a><br>
Ex Hashing:<br>
<a href="https://drive.google.com/drive/folders/1zUEvLgnr9-J1OdzWTlb5jMfWlrzD8JJp?usp=sharing">https://drive.google.com/drive/folders/1zUEvLgnr9-J1OdzWTlb5jMfWlrzD8JJp?usp=sharing</a></p>

