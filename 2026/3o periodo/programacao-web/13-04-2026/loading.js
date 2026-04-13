async function carregarPaginas() {
  try {
    const navbarResponse = await fetch('navbar.html');
    const navbarHTML = await navbarResponse.text();
    const navbarContainer = document.querySelector('#navbar-container');
    const navbarBody = new DOMParser().parseFromString(navbarHTML, 'text/html').body.innerHTML;
    navbarContainer.innerHTML = navbarBody;

    const carouselResponse = await fetch('carousel.html');
    const carouselHTML = await carouselResponse.text();
    const carouselContainer = document.querySelector('#carousel-container');
    const carouselBody = new DOMParser().parseFromString(carouselHTML, 'text/html').body.innerHTML;
    carouselContainer.innerHTML = carouselBody;

    const produtosResponse = await fetch('produtos.html');
    const produtosHTML = await produtosResponse.text();
    const produtosContainer = document.querySelector('#produtos-container');
    const produtosBody = new DOMParser().parseFromString(produtosHTML, 'text/html').body.innerHTML;
    produtosContainer.innerHTML = produtosBody;

    const footerResponse = await fetch('footer.html');
    const footerHTML = await footerResponse.text();
    const footerContainer = document.querySelector('#footer-container');
    const footerBody = new DOMParser().parseFromString(footerHTML, 'text/html').body.innerHTML;
    footerContainer.innerHTML = footerBody;
  } catch (error) {
    console.error('Erro ao carregar as páginas:', error);
  }
}

document.addEventListener('DOMContentLoaded', carregarPaginas);