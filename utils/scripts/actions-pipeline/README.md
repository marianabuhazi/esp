### Setting Up Pre-push Hook

To ensure code formatting compliance, we provide a pre-push Git hook that runs before you push changes to the repository. Follow these steps to set it up:

1. Navigate to the root directory of the repository.
2. Run the setup script to activate the pre-push hook:
    ```bash
    ./setup.sh
    ```
    or
    ```bash
    bash setup.sh
    ```
    This script will automatically copy the pre-push hook to the appropriate location.

Now, every time you try to push changes to the repository, the pre-push hook will run automatically to check code formatting compliance.
