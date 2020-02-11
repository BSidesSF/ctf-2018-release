package main

import (
        "fmt"
	"encoding/json"
        "html/template"
	"io/ioutil"
        "os/exec"
	"net/http"
        "strconv"
        "strings"

	"github.com/gorilla/mux"
)

type Crossword struct {
	Width    int
	Height   int
	Fields   []Field
	Wordlist map[string]int
}

type Field struct {
	Row       int
	Col       int
	Length    int
	Direction string
}

type Response struct {
    Status string
    Message string
    Results []string
}

var crossword Crossword
var flag string

const phpBinary string = "/usr/bin/php5"
const flagPath string = "/flag"

func IndexHandler(w http.ResponseWriter, r *http.Request) {
    t, _ := template.ParseFiles("templates/index.html")
    t.Execute(w, "")
}

func CheckHandler(w http.ResponseWriter, r *http.Request) {
    r.ParseForm()
    var answers map[string]map[string]string
    err := json.Unmarshal([]byte(r.FormValue("answers")), &answers)
    if err != nil {
        http.Error(w, err.Error(), 500)
        return
    }
    w.Header().Set("Content-Type", "application/json")

    if !validCharacters(answers) {
        json.NewEncoder(w).Encode(Response{"error", "invalid characters", []string{}})
        return
    }
    results :=make([]string, len(crossword.Fields))
    for i, f := range(crossword.Fields) {
        row := f.Row; col := f.Col
        expr := make([]string, f.Length)
        for j := 0; j < f.Length; j++ {
            var ok bool
            expr[j], ok = answers[strconv.Itoa(row)][strconv.Itoa(col)]

            // incomplete expression
            if !ok  {
                results[i] = "incomplete"
                break
            }

            // complete expression - see if it works!
            if (j == f.Length - 1) {
                if validateExpression(strings.Join(expr, " ")) {
                    results[i] = "correct"
                } else {
                    results[i] = "incorrect"
                }
                break
            }

            if f.Direction == "across" {
                col += 1
            } else if f.Direction == "down" {
                row += 1
            }
        }
    }

    flagGet := true
    for _, r := range(results) {
        if r != "correct" {
            flagGet = false
        }
    }
    if flagGet {
        json.NewEncoder(w).Encode(Response{"flag", flag, results})
        return
    }
    json.NewEncoder(w).Encode(Response{"ok", "", results})
}

func validCharacters(answers map[string]map[string]string) bool {
    // TODO write a test for this
    counts := make(map[string]int)
    for _, col := range(answers){
        for _, token := range(col){
            if _, ok := crossword.Wordlist[token]; !ok {
                return false
            }
            counts[token] += 1
            if counts[token] > crossword.Wordlist[token] {
                return false
            }
        }
    }
    return true
}

func validateExpression(expr string) bool {
    fmt.Println("Validating " + expr)
    statement := fmt.Sprintf("assert (%s); echo \"success\";", expr)
    fmt.Println(statement)
    if out, err := exec.Command(phpBinary, "--define", "assert.bail", "-r", statement).Output(); err == nil {
        fmt.Println(string(out))
        if string(out) == "success" {
            return true
        }
    }
    return false
}

func main() {
	raw, _ := ioutil.ReadFile("./static/crossword.json")
        f, err:= ioutil.ReadFile(flagPath)
        if err !=nil {
            return
        }
        flag = string(f)
	json.Unmarshal(raw, &crossword)

	r := mux.NewRouter()
	r.PathPrefix("/static/").Handler(http.StripPrefix("/static/", http.FileServer(http.Dir("./static"))))
	r.HandleFunc("/", IndexHandler).Methods("GET")
        r.HandleFunc("/check", CheckHandler).Methods("POST")

        http.ListenAndServe(":8080", r)
}
