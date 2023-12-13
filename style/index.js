//------------K-Map Fetch Function----------------------//

function Draw(container_id, ex_filename) {
	var container = document.getElementById(container_id);
	if (container.style.display == "none") {
		fetch(ex_filename)
			.then((response) => response.text())
			.then((htmlCode) => {
				container.innerHTML = "";
				container.innerHTML = htmlCode;
				MathJax.typeset();
			})
			.catch((error) => {
				console.error("Error loading HTML:", error);
			});
		container.style.display = "block";
	} else {
		container.style.display = "none";
		return Draw(container_id, ex_filename);
	}
}

//------------Ternary-Toggle Input Function(0-->1-->X)---------------------//

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

// ---------------PHP Linking Functions-----------------//

{
	function Inputs() {
		const divElement = document.querySelectorAll("#Minterm");
		var valueofdiv = [];
		var Inputvalues = [];

		divElement.forEach((divElement) => {
			valueofdiv.push(
				divElement.textContent.trim().replace(/ /g, "").split("\n")
			);
		});

		valueofdiv.forEach((row) => {
			row.forEach((col) => {
				col = parseInt(row[1]);
				Inputvalues[col] = row[0];
			});
		});
		Output(Inputvalues);
	}

	function Output(Input) {
		var jsonArray = JSON.stringify(Input);
		fetch("backend/link.php", {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			body: jsonArray,
		})
			.then((response) => {
				if (response.ok) {
					console.log("Input Data Transferred!!");
					Minterms(Input);
					GroupMaker();
				} else {
					console.log("Input Data Tranfer Failed");
				}
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
}

//------------------LOGIC-FUNCTION-NOTATION----------------------//

function Minterms(Min) {
	var i = 0,
		j = 0,
		k = 0;
	var min = [],
		dontC = [],
		MinString;
	Min.forEach((valueofMin) => {
		if (valueofMin === "1") {
			min[j] = i.toString();
			j++;
		} else if (valueofMin === "X") {
			dontC[k] = i.toString();
			k++;
		}
		i++;
	});

	if (j === 0 && k === 0) MinString = "`summ(phi)`";
	else if (j && k === 0) MinString = "`summ(" + min.join(",") + ")`";
	else if (j && k)
		MinString =
			"`summ(" + min.join(",") + ")` `+` `d(" + dontC.join(",") + ")`";
	else MinString = MinString = "`summ(phi)+d(" + dontC.join(",") + ")`";

	document.getElementById("Min").textContent = MinString;
	MathJax.typeset();
}

//---------------Fetching Prime-Implecants------------------//

function GroupMaker() {
	fetch('backend/executables/EPI.txt')
		.then(response => response.text())
		.then(data => {
			const dataArray = data.split(/\r?\n/).filter(line => line.trim() !== '');
			booleanOutput(dataArray)
			Implicants_Input(dataArray);
		})
		.catch(error => {
			console.error(error);
		});
}

//---------------------------------------------------------//



//--------------EPI's to MathJAX Converter----------------//
function booleanOutput(minterms) {
	let Mathjax_Output = [];
	let sor = minterms.length;
	let soc = minterms[0].length;
	let index = 0;
	if (soc == 1) {
		Mathjax_Output = '0';
		OutputTransfer(Mathjax_Output);
		return;
	}
	for (let i = 0; i < soc; i++) {
		if (minterms[0][i] != '_') break;
		if (i === (soc - 1)) {
			Mathjax_Output = '1';
			OutputTransfer(Mathjax_Output);
			return;
		}
	}
	for (let i = 0, count = 0; i < sor; i++) {
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
//--------------------------------------------------------//


//---------------Minterms_Group_Generator-----------------//

var Groups, index;
var _weight, _1sweight;

function Implicants_Input(EPIs) {
	Groups = [];
	index = 0;
	for (let i = 0; i < EPIs.length; i++) {
		let PI = EPIs[i].match(/./g);
		Groups.push([]);
		GroupG(PI);
		index++;
	}
	console.log(Groups); // you can use this Groups array for ring logic!!
}

function GroupG(Epi) {
	_weight = [];
	_1sweight = 0;
	const _n = WeightG(Epi);
	for (let i = 0, j, k; i < _n; i++) {
		k = i;
		j = 0;
		let temp = 0;
		while (k > 0) {
			if (k % 2) temp += _weight[j];
			j++;
			k >>= 1;
		}
		temp += _1sweight;
		Groups[index].push(temp);
	}
}

function WeightG(Epi) {
	const soc = Epi.length;
	let n = 0;
	for (let i = 0; i < soc; i++) {
		switch (Epi[soc - i - 1]) {
			case '_': _weight[n] = 2 ** i;
				n++;
				break;
			case '1': _1sweight += 2 ** i;
				break;
		}
	}
	return 2 ** n;
}

//-------------------------------------------------------//
