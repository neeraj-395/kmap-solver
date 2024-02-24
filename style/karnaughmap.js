function KarnaughMapData() {
    this.nVar = 0;
    this.nRows = 0;
    this.nCols = 0;
    this.rowSeq = [];
    this.colSeq = [];
    this.cellSize = 0;
    this.nVarOnTop = 0;
    this.nVarOnLeft = 0;
}

KarnaughMapData.prototype.init = function(vars, cellSize) {
    this.nVarOnTop = Math.ceil(vars/2);
    this.nVarOnLeft = Math.floor(vars/2);
    this.nRows = Math.pow(2, this.nVarOnTop);
    this.nCols = Math.pow(2, this.nVarOnLeft);
    this.nVar = vars;
    this.cellSize = cellSize;

    let i = 0
    while(i < this.nRows || i < this.nCols){
        if(i < this.nRows){
            this.rowSeq.push(i);
        }
        if(i < this.nCols){
            this.colSeq.push(i);
        }
        i++;
    }

    if(this.nRows > 2){
        let tmp = this.rowSeq[this.nRows - 1];
        this.rowSeq[this.nRows - 1] = this.rowSeq[this.nRows - 2];
        this.rowSeq[kmap.nRows - 2] = tmp;
    }
    if(this.nCols > 2){
        let tmp = this.colSeq[this.nCols - 1];
        this.colSeq[this.nCols - 1] = this.colSeq[this.nCols - 2];
        this.colSeq[this.nCols - 2] = tmp;
    }
}

var kmap = {};

function drawKmap(vars, cellSize, container) {

    kmap = new KarnaughMapData();
    kmap.init(vars, cellSize);

    document.body.style.setProperty("--height", cellSize + "px");
    document.body.style.setProperty("--width", cellSize + "px");

    let kmapContainer = document.getElementById(container);
    let svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
    svg.setAttribute('width', (kmap.nRows * kmap.cellSize + kmap.cellSize));
    svg.setAttribute('height', (kmap.nCols * kmap.cellSize + kmap.cellSize));

    let path = document.createElementNS('http://www.w3.org/2000/svg', 'path');
    path.setAttribute("stroke-width", "2");
    path.setAttribute("stroke", 'black');
    var d = "";
    d += "M 15,15";
    d += "L " + cellSize + "," + cellSize; 
    path.setAttribute('d',d);
    svg.appendChild(path);

    if(vars <= 6) drawLiterals(svg);
  
    createMatrix(svg, minIndexSeq());
	
	kmapContainer.appendChild(svg);

	kmapContainer.innerHTML += "<br><br><br>";

    resultPanel(kmapContainer);
}

function createMatrix(svg, sequence) {
    let x1 = (kmap.cellSize/2), y1 = (kmap.cellSize / 2) + 9; 
    let minIndexAxis = (kmap.cellSize - 2);

    for (let i = 1, t = 0; i <= kmap.nCols; i++) {
        for (let j = 1; j <= kmap.nRows; j++) {
            let minInxVal = sequence[t++];

            let rect = document.createElementNS('http://www.w3.org/2000/svg', 'rect');
            rect.setAttribute('class', 'k-box');
            rect.setAttribute('x', (j) * kmap.cellSize);
            rect.setAttribute('y', (i) * kmap.cellSize);
            rect.setAttribute('onclick','toggle(\'in'+ minInxVal+'\')');
            svg.appendChild(rect);
            
            let text1 = document.createElementNS('http://www.w3.org/2000/svg','text');
            text1.setAttribute('id', "in" + minInxVal);
            text1.setAttribute('class', 'minterm');
            text1.setAttribute('x', ((j) * kmap.cellSize) + x1);
            text1.setAttribute('y', ((i) * kmap.cellSize) + y1);
            text1.setAttribute('onclick','toggle(\'in'+ minInxVal+'\')');
            text1.textContent = '0';
            svg.appendChild(text1);

            let text2 = document.createElementNS('http://www.w3.org/2000/svg', 'text');
            text2.setAttribute('class', 'minIndex');
            text2.setAttribute('x', ((j) * kmap.cellSize) + minIndexAxis);
            text2.setAttribute('y', ((i) * kmap.cellSize) + minIndexAxis);
            text2.textContent = minInxVal;
            svg.appendChild(text2);
        }
    }
}

function drawLiterals(svg) {
    let leftLiterals = document.createElementNS('http://www.w3.org/2000/svg', 'foreignObject');
    leftLiterals.setAttribute('x', '0');
    leftLiterals.setAttribute('y', kmap.cellSize);
    leftLiterals.setAttribute('width', kmap.cellSize);
    leftLiterals.setAttribute('height', (kmap.nCols * kmap.cellSize));

    leftLiterals = literalsJax(kmap.nVarOnLeft, leftLiterals, 0, kmap.colSeq);
    svg.appendChild(leftLiterals);

    let topLiterals = document.createElementNS('http://www.w3.org/2000/svg', 'foreignObject');
    topLiterals.setAttribute('x', kmap.cellSize);
    topLiterals.setAttribute('y', '0');
    topLiterals.setAttribute('width', (kmap.nRows * kmap.cellSize));
    topLiterals.setAttribute('height', kmap.cellSize);

    topLiterals = literalsJax(kmap.nVarOnTop, topLiterals, kmap.nVarOnLeft, kmap.rowSeq);
    svg.appendChild(topLiterals);
}

function literalsJax(numOfVar, literals, startFrom, seq) {
    seq.forEach(val => {
        var div = document.createElement('div');
        div.setAttribute('class', 'alignMin');

        let tmp = val.toString(2).padStart(numOfVar, '0');
        let text = "`";
        for(let j = (numOfVar - 1), k = 0; j >= 0 ; j--){
            switch(tmp[k]){
                case '1': text += String.fromCharCode(65 + (k++) + startFrom);
                break;
                case '0' : text += "bar" + String.fromCharCode(65 + (k++) + startFrom);
                break;
            }
        }
        text += '`';
        div.textContent = text;
		literals.appendChild(div);
    });
    return literals;
}

function minIndexSeq(){
    let requiredSeq = [];

    kmap.colSeq.forEach(val1 => {
        let str1 = val1.toString(2).padStart(kmap.nVarOnLeft, '0');
        kmap.rowSeq.forEach(val2 => {
            let str2 = val2.toString(2).padStart(kmap.nVarOnTop, '0');
            requiredSeq.push(parseInt(str1+str2, 2));
        });
    });
    
    return requiredSeq;
}

function resultPanel(container){
    let div = document.createElement('div');
    div.setAttribute('class', 'result');
    let f = "`f(";
    for (let i = 0; i < kmap.nVar; i++) {
        f += String.fromCharCode(65 + i);
        if(i != (kmap.nVar - 1)) f += ",";
    }
    f += ")\\Rightarrow`";
    div.innerHTML = "<p style=\"overflow: break-word !important; font-size: 18px; text-align: center;\">" + 
        f + "&nbsp;<span style=\"font-size: 20px;\" id=\"output\">0</span></p>";

    container.appendChild(div);
}