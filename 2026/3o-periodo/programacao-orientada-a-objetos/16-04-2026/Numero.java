public class Numero {
	public static void main(String args[]) {
		int maior = Integer.MAX_VALUE;
		int menor = Integer.MIN_VALUE;

		float soma = 0;
		double media;

		for (int i = 0; i < args.length; i++) {
			int atual = Integer.parseInt(args[i]);
			if (atual > maior) {
				maior = atual;
			}
			if (atual < menor) {
				menor = atual;
			}
		}
		media = soma / args.length;
		System.out.printf("Maior: %d, menor: %d, média %f", maior, menor, media);
	}
}