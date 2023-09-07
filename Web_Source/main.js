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
    document.getElementById(id).innerHTML = "1";
}
function call2(id) {
    document.getElementById(id).innerHTML = "X";
}