package main

import (
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
)

type SensorData struct {
	Temperatura float64 `json:"temperatura"`
	Umidade     float64 `json:"umidade"`
	Timestamp   string  `json:"timestamp"`
}

func getData() (*SensorData, error) {
	resp, err := http.Get("")
	if err != nil {
		return nil, err
	}

	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	var data SensorData
	err = json.Unmarshal(body, &data)
	if err != nil {
		return nil, err
	}

	return &data, nil
}

func handler(w http.ResponseWriter, r *http.Request) {
	data, err := getData()
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
		return
	}

	log.Printf("Temperatura: %.2f *C | Umidade: %.2f %%, Timestamp: %s\n",
		data.Temperatura, data.Umidade, data.Timestamp)
	fmt.Fprintf(w, "Dados recebidos.")
}

func main() {
	http.HandleFunc("/dados", handler)
	log.Println("Servidor na porta 8080.")
	log.Fatal(http.ListenAndServe(":8080", nil))
}
