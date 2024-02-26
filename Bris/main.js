var piezas = document.getElementsByClassName('movil');
var tamWidth =[134,192,134,163,134,163,134,192,134];
var tamHeight = [163,134,163,134,192,134,163,134,163];

var elementSelect = null;
var currentX = 0;
var currentY = 0;
var currentPosX = 0;
var currentPosY = 0;

// Asignar eventos de mousedown a las piezas
for(var i = 0; i < piezas.length; i++){
    piezas[i].setAttribute("width", tamWidth[i]);
    piezas[i].setAttribute("height", tamHeight[i]);
    piezas[i].setAttribute("x", Math.floor((Math.random()*10)+1));
    piezas[i].setAttribute("y", Math.floor((Math.random()*409)+1));
    piezas[i].addEventListener("mousedown", seleccionarElemento);
}

function seleccionarElemento(evt) {
    elementSelect = evt.target;
    currentX = evt.clientX;
    currentY = evt.clientY;
    currentPosX = parseFloat(elementSelect.getAttribute("x"));
    currentPosY = parseFloat(elementSelect.getAttribute("y")); 
    elementSelect.addEventListener("mousemove", moverElemento);
    document.addEventListener("mouseup", deseleccionarElemento);
}

function moverElemento(evt) {
    var dx = evt.clientX - currentX;
    var dy = evt.clientY - currentY;
    currentPosX += dx;
    currentPosY += dy;
    elementSelect.setAttribute("x", currentPosX);
    elementSelect.setAttribute("y", currentPosY);
    currentX = evt.clientX;
    currentY = evt.clientY;
}

function deseleccionarElemento(evt) {
    elementSelect.removeEventListener("mousemove", moverElemento);
    elementSelect = null;
    testing();
}

function testing() {
    var bienUbicada = 0;
    for(var i = 0; i < piezas.length; i++) {
        var posx = parseFloat(piezas[i].getAttribute("x"));
        var posy = parseFloat(piezas[i].getAttribute("y"));
        if(Math.abs(posx - origX[i]) < 15 && Math.abs(posy - origY[i]) < 15) {
            bienUbicada++;
        }
    }
    if(bienUbicada == 9) {
        document.getElementById('siguienteNivelBtn'.style.display='block');
        win[0].play();
    }
}
