function Click(container_id,ex_filename){
    var container = document.getElementById(container_id);
    if(container.style.display=='none'){
        fetch(ex_filename)
        .then(response => response.text())
        .then(htmlCode => {
            container.innerHTML="";
            container.innerHTML = htmlCode;
            MathJax.typeset();
        })
        .catch(error => {
            console.error("Error loading HTML:", error);
        });
        container.style.display='block';
    }
    else{
        container.style.display='none';
        return Click(container_id,ex_filename);
    }
    
};

function call(id) {
    var Min = document.getElementById(id).innerHTML;
    if(Min ==='0'){
      document.getElementById(id).innerHTML='1';
    }
    else if(Min === '1'){
      document.getElementById(id).innerHTML='X';
    }
    else{
      document.getElementById(id).innerHTML='0';
    }
}
var abar="\\"+"["+"\\"+"bar{A}"+"\\"+"]";
var bbar="\\"+"["+"\\"+"bar{B}"+"\\"+"]";
var cbar="\\"+"["+"\\"+"bar{C}"+"\\"+"]";
var dbar="\\"+"["+"\\"+"bar{D}"+"\\"+"]";
var input=["(","A'","^","B",")","v","(","A","^","B'",")","v","(","C'","^","D",")","v","(","C","^","D'",")"];
var Min=getElementById('res85');
for (let i = 0; i < 1000; i++) {
  if (input[i] == "A'") {
    input[i] = abar;
  }
  if (input[i] == "B'") {
    input[i] = bbar;
  }

  if (input[i] == "C'") {
    input[i] = cbar;
  }
  if (input[i] == "D'") {
    input[i] = dbar;
  }
  if (input[i] == "^") {
    input[i] = ".";
  }
  if (input[i] == "v") {
    input[i] = "+";
  }
}
Min=input;

