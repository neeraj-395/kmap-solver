//------------K-Map Fetch Function----------------------//

function Click(container_id, ex_filename) {
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
		return Click(container_id, ex_filename);
	}
}

//------------Ternary-Toggle Input Function(0-->1-->X)---------------------//

function call(id) {
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
		Minterms(Inputvalues);
		Output(Inputvalues);
	}

	function Output(Input) {
		var jsonArray = JSON.stringify(Input);
		fetch("Link.php", {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			body: jsonArray,
		})
			.then((response) => {
				if (response.ok) {
					console.log("Input Data Transferred!!");
					OutputTransfer();
				} else {
					console.log("Input Data Tranfer Failed");
				}
			})
			.catch((error) => {
				console.error("Error:", error);
			});
	}

	function OutputTransfer() {
		const outputDiv = document.getElementById("output");

		fetch('PHP_Executable/MathJax.txt')
			.then((response) => {
				if (!response.ok) {
					throw new Error("Network response was not ok");
				}
				return response.text();
			})
			.then((fileContent) => {
				outputDiv.textContent = fileContent;
				MathJax.typeset();
				GroupMaker();
			})
			.catch((error) => {
				console.error("Error:", error);
				outputDiv.textContent = "Oops thier is a Problem!!";
			});
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
	fetch('PHP_Executable/Implicants.txt')
		.then(response => response.text())
		.then(data => {
			const dataArray = data.split(/\r?\n/).filter(line => line.trim() !== '');
			Implicants_Input(dataArray);
		})
		.catch(error => {
			console.error(error);
		});
}

//---------------Minterms_Group_Generator-----------------//

var Groups;
var index;

function Implicants_Input(EPIs) {
	Groups = [];
	index = 0;
	for (let i = 0; i < EPIs.length; i++) {
		let PI = [];
		PI = EPIs[i].match(/./g);
		Groups.push([]);
		WeightG(PI);
		index++;
	}
	console.log(Groups); // you can use this Group array to check the the group should to ringed!!
}
function WeightG(Epi) {
	const UB = Epi.length;
	let _weight = [];
	let _1sweight = 0;
	for (var i = 0, _N = 0; i < UB; i++) {
		if (Epi[i] === "_") {
			_weight[_N] = Math.pow(2, UB - 1 - i);
			_N++;
		} else if (Epi[i] === "1") {
			_1sweight += Math.pow(2, UB - 1 - i);
		}
	}
	Bi_Pattern(_N, _weight, _1sweight);
}
function GroupG(bi_pattern, _weight, _1sweight) {
	let temp = 0;
	for (let i = 0; i < bi_pattern.length; i++) {
		if (bi_pattern[i] === "1") {
			temp += _weight[i];
		}
	}
	temp += _1sweight;
	Groups[index].push(temp);
}
function Bi_Pattern(n, _weight, _1sweight, pattern = "") {
	if (n === 0) {
		GroupG(pattern, _weight, _1sweight);
	} else {
		Bi_Pattern(n - 1, _weight, _1sweight, pattern + "0");
		Bi_Pattern(n - 1, _weight, _1sweight, pattern + "1");
	}
}

//-------------------------------------------------------//