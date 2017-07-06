/* go run hola.go
jenni@Coeur:~/go/src/prueba$ go build hola.go
jenni@Coeur:~/go/src/prueba$ ./hola j k l
*/
package main
import "fmt"
import "os"
import "io/ioutil"
import "strings"
import "sort"

var mymap = make(map[string]int)
var mp = make(map[int]string)

func check(e error){
	if e != nil{
		panic(e)
	}
}

func main()	{
	args:=os.Args[1:]
	numfiles:= len(args)
	if numfiles < 1 {
		fmt.Println("Please in order to the correct use, add one or more file/s")
	} else {

		for	_,file:= range args	{
			data,err :=	ioutil.ReadFile(file)
			check(err)
		for	_, line := range strings.Split(string(data)," "){
				mymap[line]++
			}
		}

		var keys []string
		for k := range mymap {
    		keys = append(keys, k)
		}
		sort.Strings(keys)

		for _, k := range keys {
			if mymap[k]>0 {
    			fmt.Println("\n",k, mymap[k])
	   		}
    	}
	
	}
}
