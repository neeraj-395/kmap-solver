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

//------------Toggle Input Function---------------------//
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

// ---------------PHP Linking Function-----------------//
{
function Inputs(){
var textBlocks = document.querySelectorAll('.K-Map_Input');

var valuesArray = [];

textBlocks.forEach(function(text) {
  valuesArray.push(text.textContent);
});
Output(valuesArray);
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
//----------------End of Functions-----------------------//