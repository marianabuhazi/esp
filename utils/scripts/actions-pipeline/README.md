# ESP Linting and Regression Testing Workflows
The scripts included in this directory are used as part of a GitHub Actions workflow triggered on PRs to the dev, main or master branches.

## Scripts
### Code Formatting
There are two scripts under [./code-format](https://github.com/marianabuhazi/esp/blob/scripts-05-07/utils/scripts/actions-pipeline/code-format/) that can be used by contributors to check their code for formatting violations, as well as to format code in place. These scripts could also be used as part of a Git hook or a GitHub Actions workflow.
The scripts support formatting for the following languages: C/C++, Verilog/SystemVerilog, VHDL, and Python. 

#### Format Modified    
Bash script that examines the Git status of the remote repository to identify files that the user has recently modified, added, or deleted. It then determines the file extension type (e.g., C/C++, VHDL, SystemVerilog/Verilog, or Python) and invokes an open-source linting tool for that language with the filename and other necessary arguments. The script offers flexibility through various flags, allowing users to both report programming violations and fix formatting in-place. This tool plays a crucial role in maintaining ESP's readability, consistency, and bug-free nature. 
``` 
ESP format checker ✨🛠️
Report violations or format files in-place.
Usage: ./format_modified [OPTIONS]
    -h            Display this help message
    -f <file>     Fix formatting for file <file>
    -c <file>     Check formatting for file <file>
    -a            Apply to all
    -g            Run as Github Actions workflow or pre-push hook
```

#### Format Repo
The Format Modified script is useful for formatting new modifications to existing code. However, this implies that the code in the repository is already properly structured to begin with. In reality, our ESP repository lacks uniformity. To introduce formatting into the existing code, we have provided another script called Format Repo which traverses through all directories in the ESP repository recursively, skipping over any directories that are Git submodules owned by another organization. It identifies the file extension of each file within the current directory and formats it in-place by calling the appropriate open-source linter. 
Format Repo is intended for one-time use. Format Repo can significantly impact Git blame, potentially attributing all changes to a single contributor (whoever ran the script). To mitigate this, changes made by Format Repo should be committed separately and excluded from the blame.
``` 
ESP format checker ✨🛠️
Recursively format all files in the /esp repository for a given file extension.
Usage: [-t {c, cpp, vhdl, v, py}]
Options:
    -h, --help                        Display this help message.
    -t, --type {c, cpp, vhdl, v, py}  Specify the type of files to format.
```

### Regression Testing

#### Get Modified Accelerators
This script accepts a configuration file called accelerators.json and uses the repository's Git history to determine modified accelerators. The configuration file is simply a JSON file where ESP members can specify what accelerators they wish to test. Imagine that a user makes modifications to many accelerators in a single Pull Request. We may only care about testing a subset of these at a given time.
By default the get_modified_accelerators.sh scripts can run end-to-end tests with each modified accelerator in a 2x2 SoC, but having the configuration file adds extra flexibility to define/override which accelerators are to be tested.

The script compares the modified files (as specified in the Git history) to the accelerators in the configuration file. It then returns a list of the overlap of accelerators that should be tested, along with references to their HLS and behavioral `make` commands (ie. `make cholesky_stratus-hls` or `make cholesky_stratus-beh`).

#### Run Simulations
The Run Simulations script accepts a list of testable accelerators via a bash list, and runs HLS for each one. The script first assesses how many accelerators have been scheduled for synthesis.
We consider that in reality a user may only modify one or two accelerators at a time, but designed this project such that if many accelerators are modified in one Pull Request
the program can balance them all effectively across different CPUs on the SOCP server and execute them in parallel. The script iterates through the list of accelerators
and starts and HLS run for each one, asynchronously. Then, it waits for all HLS jobs to return, logging the results for each job as they complete. Once all jobs have
completed, the script exits with success or error code.

#### Run ESP Config
After the Run Simulations scripts executes, the Run ESP Config script should be executed. Run ESP Config is a bash script that completes the bitstream generation,
FPGA programming, and the test program execution on one of the ESP Xilinx FPGAs. Run ESP Config first looks through the tech/acc folder in the working repository. 
This directory has all the work folders for the completed HLS jobs. The script iterates through all the successful accelerator HLS jobs and creates an ESP 2x2 SoC configuration 
with that accelerator in the accelerator tile. The script then proceeds to run `make esp-config` and `make vivado-syn` to generate the HDL and bitstream for the 
full SoC configuration. If this succeeds, the script will continue to run `make fpga-program` to program the generated bitstream on the FPGA, and execute a test
program using `make fpga-run`. The script opens up a minicom connection to the FPGA simultaneously to running `make fpga-run`. The results of the test program
are read out from the minicom to a log file for users to refer to after completion. 

If more than one accelerator has been modified, the script will start a new end-to-end test by creating the ESP configuration, generating HDL, generating the bitstream,
configuring the FPGA and running the test program with the next accelerator, and so on.


## Setting Up Pre-push Hook

To ensure code formatting compliance, we provide a pre-push Git hook that runs before you push changes to the repository. Follow these steps to set it up:

1. Navigate to the root directory of the repository.
2. Run the setup script to activate the pre-push hook:
    ```bash
    .githooks/./setup.sh
    ```
    or
    ```bash
    bash .githooks/setup.sh
    ```
    This script will automatically copy the pre-push hook to the appropriate location.

Now, every time you try to push changes to the repository, the pre-push hook will run automatically to check code formatting compliance.


## Setting Up Self-Hosted GitHub Runners
- [Adding self-hosted runners to the repository](https://docs.github.com/en/actions/hosting-your-own-runners/managing-self-hosted-runners/adding-self-hosted-runners)
- [Configuring and starting the runner](https://docs.github.com/en/actions/hosting-your-own-runners/managing-self-hosted-runners/adding-self-hosted-runners)


## Writing GitHub Actions YAML workflows
- [Writing on pull_request workflows](https://docs.github.com/en/actions/using-workflows/workflow-syntax-for-github-actions)
