package parteUm.dominio;

public class Helicoptero {
    private String modelo;
    private String sigla;
    private String piloto;

    public Helicoptero(String modelo, String sigla) {
        this.modelo = modelo;
        this.sigla = sigla;
    }

    public void atribuirPiloto(String nome) {
        this.piloto = nome;
    }

    public String obterDescricaoCompleta() {
        return "Modelo: " + modelo + ", Sigla: " + sigla + ", Piloto: " + piloto;
    }
}