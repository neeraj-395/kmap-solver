//------------K-Map Fetch Function----------------------//
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
}

//------------Ternary-Toggle Input Function(0-->1-->X)---------------------//
function call(id) {
    var Min = document.getElementById(id).innerHTML;
    if(Min ==='0'){
      document.getElementById(id).innerHTML = '1';
    }
    else if(Min === '1'){
      document.getElementById(id).innerHTML = 'X';
    }
    else{
      document.getElementById(id).innerHTML = '0';
    }
}

// ---------------PHP Linking Functions-----------------//

{
function Inputs(){

const divElement = document.querySelectorAll('#Minterm');
var valueofdiv = [];
var Inputvalues = [];

divElement.forEach(divElement => {
  valueofdiv.push(divElement.textContent.trim().replace(/ /g,'').split('\n'));
});

valueofdiv.forEach(row =>{
  row.forEach(col =>{
    col = parseInt(row[1]);
    Inputvalues[col]=row[0];
  });
});
Minterms(Inputvalues);
Output(Inputvalues);
}

function Output(Input){
var jsonArray = JSON.stringify(Input);
fetch('Link.php', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/json',
  },
  body: jsonArray,
})
  .then(response => {
    if(response.ok){
    console.log("Input Data Transferred!!");
    OutputTransfer();
    return response.text();
    }
    else{
      console.log("Input Data Tranfer Failed");
    }
  })
  .then(data => {
    console.log(data);
  })
  .catch(error => {
    console.error('Error:', error);
  });
}

function OutputTransfer(){
    const outputDiv = document.getElementById('output');

    const filePath = 'PHP_Executable/Output.txt';

    fetch(filePath)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.text();
        })
        .then(fileContent => {
            outputDiv.textContent = fileContent;
            MathJax.typeset();
        })
        .catch(error => {
            console.error('Error:', error);
            outputDiv.textContent = 'Oops thier is a Problem!!';
        });
}
}

//------------------LOGIC-FUNCTION-NOTATION----------------------//

function Minterms(Min){
  var i=0,j=0,k=0;
  var min = [], dontC = [], MinString;;
  Min.forEach(valueofMin=>{
    if(valueofMin === '1'){
      min[j] = i.toString();
      j++;
    }
    else if(valueofMin==='X'){
      dontC[k] = i.toString();
      k++;
    }
    i++;
  });
  
  if(j === 0 && k === 0) MinString = "`m\sum(phi)`";
  else if(j && k === 0) MinString = "`m\sum(" + min.join(',') + ")`"
  else if(j && k) MinString = "`m\sum(" + min.join(',') + ")` `+` `d(" + dontC.join(',') + ")`";
  else MinString = MinString = "`m\sum(phi)+d(" + dontC.join(',') + ")`";
  

  document.getElementById('Min').textContent = MinString;
  MathJax.typeset();
}
//----------------End of Functions-----------------------//