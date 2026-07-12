package parteDois.dominio;

public class Helicoptero implements Veiculo {
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

    @Override
    public String obterDescricao() {
        return "Modelo: " + modelo + ", Sigla: " + sigla + ", Piloto: " + piloto;
    }
}