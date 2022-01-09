# OpenTube: Collaborative Robot YuMi analyzes test samples.

## Requirements:

**Software:**
```bash
B&R Automation Studio (Version: V4.9.3.144 SP)
AS Runtime: B4.82
mapp View: 5.14.2

ABB RobotStudio 2021.2 (Version: 21.2.9526.0)
RobotWare Dual-ARM: 6.12.02
RobotWare Single-ARM: 7.3.0
```

| Software/Package           | Link                                                                                  |
| -------------------------- | ------------------------------------------------------------------------------------- |
| ABB RobotStudio            | https://new.abb.com/products/robotics/robotstudio/downloads                           |
| B&R Automation             | https://www.br-automation.com/en/downloads/#categories=Software-1344987434933         |


## Project Description:

The design and implementation of the entire system was completed in a five-member team at Institute of Automation and Computer Science, Faculty of Mechanical Engineering, Brno University of Technology in less than four weeks.

The laboratory technician sets up the test tubes, activates the system and after that all is autonomous.  The robotic workplace called openTube was created at the request of the University Hospital Brno, which is planning its pilot deployment and testing. One of the possible applications of its use is the preparation of samples taken from patients with suspected COVID-19.

We would like to thank our partners SMC, B&R Automation and ABB Robotics for their excellent cooperation and support.

The project was realized at the Institute of Automation and Computer Science, Brno University of Technology, Faculty of Mechanical Engineering (NETME Centre - Cybernetics and Robotics Division).

<p align="center">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/presentation_1.png" width="550" height="325">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/presentation_2.png" width="550" height="325">
</p>

**Unpacking a station (/ABB_Robot_Studio/*.rspag):**
1. On the File tab, click Open and then browse to the folder and select the Pack&Go file, the Unpack & Work wizard opens.
2. In the Welcome to the Unpack & Work Wizard page, click Next.
3. In the Select package page, click Browse and then select the Pack & Go file to unpack and the Target folder. Click Next.
4. In the Library handling page select the target library. Two options are available, Load files from local PC or Load files from Pack & Go. Click the option to select the location for loading the required files, and click Next.
5. In the Virtual Controller page, select the RobotWare version and then click Locations to access the RobotWare Add-in and Media pool folders. Optionally, select the check box to automatically restore backup. Click Next.
6. In the Ready to unpack page, review the information and then click Finish.

**For more information, see the following links (Czech language only):**

Youtube (Brno University of Technology): 

https://www.youtube.com/watch?v=o7Yv20GCgag

The University Hospital Brno: 

https://www.fnbrno.cz/robot-z-vut-usetri-praci-laborantum-pomahat-by-mohl-se-vzorky-covid-19-ve-fn-brno/t6854

Faculty of Mechanical Engineering (EN): 

https://www.fme.vutbr.cz/en/fakulta/aktuality/67863

CT24: 

https://www.ceskatelevize.cz/porady/1097181328-udalosti/220411000100514/video/767623

Czech radio:

https://brno.rozhlas.cz/vedci-z-brnenskeho-vut-navrhli-roboticke-pracoviste-pomuze-laborantum-zpracovat-8205399

<p align="center">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/CT.png" width="800" height="450">
</p>

## Project Hierarchy:

**Repositary [/OpenTube_Version_ABB/]:**
```bash
Industrial Robot
[ Individual projects for ABB robots (single-arm, double-arm) ] /ABB_Robot_Studio/

Programmable logic controller(PLC)
[ B&R Automation Studio Project ] /BaR_Automation_Studio/

Computer-aided design (CAD)
[ Individual designs of robotic workplaces ] /CAD/
```

## Application:

**Wiring Diagram:**

<p align="center">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/wiring_scheme.png" width="800" height="450">
</p>

**3D Print:**

<p align="center">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/3d_print.png" width="800" height="450">
</p>

**ABB RobotStudio:**

<p align="center">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/yumi_dual_arm_ph1.PNG" width="800" height="450">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/yumi_single_arm_ph1_vision1.PNG" width="800" height="450">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/yumi_single_arm_ph1_vision2.PNG" width="800" height="450">
 <img src="https://github.com/rparak/OpenTube_Version_ABB/blob/main/images/yumi_single_arm_ph1_vision3.PNG" width="800" height="450">
</p>

## Result:
Youtube: https://www.youtube.com/watch?v=WiMssWtnBL8&t=161s

## Contact Info:
Roman.Parak@outlook.com

## Citation (BibTex)
```bash
@misc{RomanParak_Bezier,
  author = {Roman Parak, Radomil Matousek, Ladislav Dobrovsky, Pavel Heriban, Zdenek Cejpek},
  title = {OpenTube: Collaborative Robot YuMi analyzes test samples},
  year = {2021},
  publisher = {GitHub},
  journal = {GitHub repository},
  howpublished = {\url{https://github.com/rparak/OpenTube_Version_ABB}}
}
```
## License
[MIT](https://choosealicense.com/licenses/mit/)
