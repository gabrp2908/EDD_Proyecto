En este repositorio se encuentra el proyecto de la materia Estructura Dinámica de Datos - 2025A, realizado por Erick Díaz (CI.29963164) y Gabriela Rey (CI. 29645414).

Comandos
1. g++ -o bin/ArbolClan.exe src/main.cpp
2. cd bin
3. ./"ArbolClan.exe"

En reino del oriente existe una familia que ha mantenido segura al shogun por generaciones, y tanto el shogunato como los lideres del clan han mantenido registros de todas las generaciones y lideres, registrando tambien las personas que han colaborado con los lideres y el shogun. Con la apertura de las fronteras la tecnologia ha avanzado y el clan quiere digitalizar sus registros y automatizar la selección de sucesores.

El clan ha dado los siguientes requerimientos e instrucciones:

1. Convertir desde un .csv los datos en un arbol binario, el .csv tiene las siguientes columnas:
- id,name, last_name, gender, age, id_father, is_dead, was_chief, is_chief.
- Solo hay hombres o mujeres (H o M).
- Los valores para was_chief, is_chief, is_chief es cero o uno. Uno, afirmativo; 0, negativo

2. Mostrar la linea de sucesión actual (solo a los vivos).

3. Asignar un nodo como lider de manera automática en caso de morir el lider actual, las condiciones de asignación son:
- Si el lider murió y tiene hijos, el lider es el primer hijo varon vivo encontrado en su árbol.
- Si el lider murió y solo tiene hembras, el lider es el primer hijo varon de la primera hija viva encontrado en su árbol.
- Si el lider murió y no tiene hijos, el lider es el primer hijo varon vivo encontrado del árbol de su hermano/hermana.
- Si el hermano/hermana esta vivo/a y no tiene hijos, el hermano/hermana se vuelve lider.
- Si el lider murió y no tiene hermanos ni hijos, el lider es el primer hijo varon vivo encontrado del árbol de su tío/a.
- Si su tío/a esta vivo/a y no tiene hijos, su tío/a se vuelve lider.
- Si el lider murió y no se puede hallar ni su tío ni su hermano, buscar el primer ancestro con dos hijos y elegir como lider el primer hijo varon vivo del árbol del ancestro.
- Si todos los hijos varones están muertos, seguir las mismas reglas para los hijas, empezando desde la primera hallada desde el lider actual, es decir, tratar a las hijas como hijos, hasta que se logre cumplir otra vez la condición de existir un hijo varon.
- Si un lider paso de los 70 años, el liderazgo se pasa al primer hijo varon vivo de su árbol.

4. Cambiar los datos de cualquier nodo del árbol, menos el id y id del padre.

5. Cada nodo debe tener una lista de contribuidores del lider donde cada elemento debe tener: nombre del contribuidor, edad actual, id, descripción de su contribución, grado de contribución al clan (rango de 1 a 10), y deben de estar ordenados según el grado de contribución y edad. Los datos se consiguen de un .csv aparte
