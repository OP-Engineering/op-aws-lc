#!/bin/bash

# Function to check if a cargo package is installed
is_cargo_installed() {
  [ -x "$HOME/.cargo/bin/$1" ]
}

# Install cargo-ndk if not installed
if ! is_cargo_installed "cargo-ndk"; then
  echo "Installing cargo-ndk..."
  cargo install cargo-ndk
else
  echo "cargo-ndk is already installed."
fi

# Install cargo-bump if not installed
if ! is_cargo_installed "cargo-bump"; then
  echo "Installing cargo-bump..."
  cargo install cargo-bump
else
  echo "cargo-bump is already installed."
fi
