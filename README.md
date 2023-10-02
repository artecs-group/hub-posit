# HUB Meets Posit: Arithmetic Units Implementation

This GitHub repository houses the codebase utilized in the experiments presented in the scientific paper titled ["HUB Meets Posit: Arithmetic Units Implementation"](https://doi.org/10.1109/TCSII.2023.3307488). The purpose of this repository is to provide a comprehensive and accessible resource for individuals interested in replicating the experiments detailed in the paper.

The scientific paper explores the intersection of HUB architectures and Posit arithmetic units, presenting novel implementations and insights. The code within this repository serves as a practical guide for researchers, developers, and enthusiasts eager to reproduce and build upon the experiments conducted in the paper.

## Getting Started

To get started with replicating the experiments, follow these steps:

1. **Clone the Repository:**
   ```
   git clone https://github.com/RaulMurillo/hub-posit.git
   cd hub-posit
   ```

2. **Install Dependencies:**

    - For numerical experiments, [Universal Numbers Library](https://github.com/stillwater-sc/universal/tree/main) is required.
    - For ASIC synthesis, Synopsys Design Compiler is used. Synthesys results with a 45nm standard cell library by TSMC are provided.
    - Python 3 and common packages are required for plot scripts.

3. **Run Experiments:**

    Move inside each directory for more instructions on how to execute the experiments.

## Structure

The repository is organized as follows:

- **`numeric/`**: Contains the source code for the numeric experiments comparing both standard and HUB posit formats.
- **`synth/`**: Contains the VHDL source code for the implementations and synthesis scripts.


## Citation

If you use this code or find the results useful in your research, please cite the paper:
> R. Murillo, J. Hormigo, A. A. D. Barrio and G. Botella, "HUB Meets Posit: Arithmetic Units Implementation," in IEEE Transactions on Circuits and Systems II: Express Briefs, doi: [10.1109/TCSII.2023.3307488](https://doi.org/10.1109/TCSII.2023.3307488).

```
@article{Murillo2023HUB,
  author={Murillo, Raul and Hormigo, Javier and Barrio, Alberto A. Del and Botella, Guillermo},
  journal={IEEE Transactions on Circuits and Systems II: Express Briefs}, 
  title={{HUB Meets Posit: Arithmetic Units Implementation}}, 
  year={2023},
  volume={},
  number={},
  pages={},
  doi={10.1109/TCSII.2023.3307488}
}
```

## License

This project is licensed under the [GPL License](LICENSE) - see the LICENSE file for details.

## Acknowledgments

This work is supported partially by grant PID2019-105396RB-I00 and PID2021-123041OB-I00, funded by MCIN/AEI/ 10.13039/501100011033, and by “ERDF A way of making Europe”, by a 2020 Leonardo Grant for Researchers and Cultural Creators, from BBVA Foundation, whose id is PR2003 20/01, by the CM under grant S2018/TCS-4423, by the Fondo Europeo de Desarrollo Regional (UMA20-FEDERJA-059) and by Junta de Andalucía under project P18-FR-3130.