package 2026.3o periodo.programacao-web.07-05-2026.br.univali.br.zoo;

public class Preguica extends Animal {  
    public Preguica(int idade, String nome) {
        super(idade, nome);
    }

    @Override
    public String emitirSom() {
        return "Zzzzz";
    }
}
