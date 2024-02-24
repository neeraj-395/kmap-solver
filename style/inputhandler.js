function initKMAP(container_id) {
	var container = document.getElementById(container_id);
	var vars = document.getElementById("nVar_id").value;

	if (container.style.display == "none") {
		container.innerHTML = "";
		if(vars == 5 || vars == 6) {
			document.body.style.setProperty('--font_size', '16px');
			drawKmap(vars, 60, container_id);
		}
		else
			drawKmap(vars, 50, container_id);
		MathJax.typeset();
		container.style.display = "block";
	} else {
		container.style.display = "none";
		return initKMAP(container_id, vars);
	}
}

function toggle(id) {
	var Min = document.getElementById(id).innerHTML;
	if (Min === "0") {
		document.getElementById(id).innerHTML = "1";
	} else if (Min === "1") {
		document.getElementById(id).innerHTML = "X";
	} else {
		document.getElementById(id).innerHTML = "0";
	}
}

function Inputs() {
		let i=0;
		const minIndex = document.querySelectorAll("text.minIndex");
		const minterm = document.querySelectorAll("text.minterm");
		var Indexs = [];
		var Inputvalues = [];

		minIndex.forEach((divElement) => {
			Indexs.push(divElement.textContent);
		});
		minterm.forEach((min) => {
				Inputvalues[Indexs[i++]] = min.textContent;
		});

		if(!Inputvalues.includes('1'))
		{
			let min = ['(null)']; 
			booleanOutput(min);
			return;
		}

		let _1sDec = "";
		let XsDec = "";
		let program_arguments = "";
		for (let i = 0; i < Inputvalues.length; i++) {
			let char = Inputvalues[i];
			switch(char)
			{
				case '1': _1sDec += i.toString() + ',';
						  break;
				case 'X': XsDec += i.toString() + ',';
						  break;
			}
		}
		
		program_arguments += (_1sDec !== "") ? _1sDec.slice(0, -1) : "\"(null)\"";
		program_arguments += " dcare= "
		program_arguments += (XsDec !== "") ? XsDec.slice(0, -1) : "\"(null)\"";
		Output(program_arguments);
}

function Output(program_arg) {
		var jsonArray = JSON.stringify(program_arg);
		fetch("backend/link.php", {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			body: jsonArray,
		})
			.then(response => {
				if(response.ok){
					console.log("Input Transfer Success");
					return response.text();
				}
				else
					throw new Error("Input Transfer Failed");
			})
			
			.then(output => {
				const dataArray = output.split(/\r?\n/);
				booleanOutput(dataArray);
			})
			.catch((error) => {
				console.error("Error:", error);
			});
}

function OutputTransfer(Mathjax_Output) {
		const outputDiv = document.getElementById("output");
		outputDiv.textContent = Mathjax_Output;
		MathJax.typeset();
}

function booleanOutput(minterms) {
	let Mathjax_Output = [];
	let sor = minterms.length;
	let soc = minterms[0].length;
	let index = 0;
	
	if(minterms[0] === "(null)") return OutputTransfer('0');

	else if (!(minterms[0].includes('1')) && !(minterms[0].includes('0'))) {
		OutputTransfer('1');
		return;
	}
	
	for (let i = 0; i < sor; i++) {
		Mathjax_Output += '`(';
		index += 2;
		for (let j = 0; j < soc; j++) {
			if (minterms[i][j] === '1') {
				Mathjax_Output += String.fromCharCode(65 + j) + 'x';
				index++;
			} else if (minterms[i][j] === '0') {
				Mathjax_Output += 'bar' + String.fromCharCode(65 + j) + 'x';
				index += 4;
			}
			if (Mathjax_Output[index] === 'x') {
				Mathjax_Output = Mathjax_Output.slice(0, index);
				Mathjax_Output += 'cdot';
				index += 4;
			}
		}

		if (Mathjax_Output[index - 1] === 't') {
			index -= 4;
			Mathjax_Output = Mathjax_Output.slice(0, index);
		}

		Mathjax_Output += ')`';
		index += 2;

		if (i === (sor - 1))
			break;

		Mathjax_Output += '`+`';
		index += 3;
	}
	OutputTransfer(Mathjax_Output);
}