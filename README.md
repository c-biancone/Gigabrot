# Gigabrot


<!--
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url] -->
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/pinecone19/Gigabrot">
    <img src="images/5000x5000.png" alt="Logo" width="800" height="800">
  </a>

  <h3 align="center">Gigabrot</h3>

<!-- DESCRIPTION -->
  <p align="center">
    Working on an efficient method to produce a gigapixel render of the Mandelbrot set with stripe-average coloring and pseudo-neumorphic normal mapping.
    <br />
    <a href="https://github.com/pinecone19/Gigabrot"><strong>Explore the docs»</strong></a>
    <br />
    <br />
    <!-- <a href="https://github.com/github_username/repo_name">View Demo</a> -->
    ·
    <a href="https://github.com/pinecone19/Gigabrot/issues">Report Bug / Request Feature</a>
    ·
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About the project</a>
      <ul>
        <li><a href="#built-with">Built with</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements-and-references">Acknowledgements and References</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This is a _work in progress_ to allow me to practive coding in a realm that I have not been able to do before, while simultaneously exploring an area that intrigues me as a numberphile. I am most comfortable with `C`, so I have begun there and am going to explore the usage of things such as `SIMD` and `AVX` instructions, loop unraveling, and any other optimizations that I can to improve the efficency. With that, I will attempt to translate the same level of functionality to `Python`, which is a language I hope to become more familiar with using. Due to Python's slowness, I will use the `Numpy` library. Eventually, I would like to be able to write this in `CUDA` so I can run the iteration task as the kernal and parrallelize the operations even more, so that multiple-gigapixel images become feasible.
      
I chose the Mandelbrot set originally not for its simplicity to generate with code, but because I am fascianted with its connections to the Fibonacci Sequence and Logistic Map, and how each of those is delicately itertwined with nature. **Math is beautiful and scary.**

### Update 01-09-2021

I have achieved pretty much the functionality and performance I originally wanted to with basic `C` so far, with a little `OpenMP` thrown in for parallel computing on the CPU. There was a point where I thought I might have to use POSIX file manipulation but I am glad to have avoided that for the sake of portability. With that, I am looking into trying to use `OpenCL` to run the ebarassingly parallel loop in as many threads as possible on the GPU, while having the CPU do the sequential logic of file manipulation. I'm prioritizing this over CUDA because the portability will be more useful than complete optimization for an NVidia GPU. Theoretically this could queue more threads in the GPU than there are pixels per row of the image, the only trouble being memory latency and having to share memory bandwidth if the CPU is to write the data to the file. While `OpenCL` is based on C, CLion does not have native support so I have to do manual configuration now :/.

Since the overall speed doesn't matter too much to me - I can let my PC chew on this for a while if I want to make a big canvas print or something, and this code is easy for me to follow - the speed of the eventual `Python` implementation should also be fine. I'm not going to edit the `Assembly` output searching for every millisecond savings. Trying `OpenCL` now is mainly because I am in a groove with `C`.



### Built With

* CLion
* Visual Studio 2019



<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps:

### Prerequisites

To view the output `.ppm` files:
* [Netpbm Viewer](http://paulcuth.me.uk/netpbm-viewer/)
* [NVidia](https://developer.nvidia.com/opencl) / [Intel](https://software.intel.com/content/www/us/en/develop/tools/opencl-sdk.html) OpenCL Library (it seems that AMD removed theirs from their servers??) I hate how little documentation there is for OpenCL :(
  
**Others N/A (Just yet)**

### Installation

1. Clone the repo into the desired directory
   ```sh
   git clone https://github.com/pinecone19/Gigabrot.git
   ```
2. Open the directory from within CLion


<!-- USAGE EXAMPLES -->
## Usage

1. In `stripedMan.c`, modify the `pXmax` and `pYmax` (Resolution) and `iterationMax` parameters to the desired values.

2. Build and run the project in CLion.

3. Once the code has finished running, the output `mandelbrot.ppm` can be found within the `cmake-build` or `cmake-build-debug` directory.

4. Drag and drop this file into [Netpbm Viewer](http://paulcuth.me.uk/netpbm-viewer/).

5. If this file is to your liking, download the `.png` file.



<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/github_username/repo_name/issues) for a list of proposed features (and known issues).



<!-- CONTRIBUTING -->
## Contributing

Pls help me. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Currently unlicensed. Go buck wild. I don't know what I'm doing. See `LICENSE` for no further information at this time.



<!-- CONTACT -->
## Contact

Chris Biancone - [email](chris.biancone@gmail.com)

Project Link: [https://github.com/pinecone19/Gigabrot](https://github.com/pinecone19/Gigabrot)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements and References

* [Andreas Leonard-Calcano](https://github.com/AndreasLc1103) and [Jake Waclawski](https://github.com/jmw3638) for helping me learn Python.
* [mrange](https://gist.github.com/mrange/20fa976388167e294aa01a1266ad0a8c)
* [Zahid Akbar](https://medium.com/convergence-tech/visualize-the-mandelbrot-set-in-gigapixels-python-15e4ad459925)
* [Jakub Ochnik](https://github.com/JakubOchnik/cuda-mandelbrot)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/chris-biancone
