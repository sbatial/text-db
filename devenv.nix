{pkgs, ...}: {
  packages = with pkgs; [musl pkg-config ninja meson clang-tools llvmPackages_15.clang-unwrapped gdb];

  languages.c.enable = true;

  pre-commit.hooks.clang-format.enable = true;

  env.DEFAULT_FILE = "main";

  scripts = {
    compile.exec = "${pkgs.meson}/bin/meson compile -C build";
    runit.exec = "echo 'Running '$DEFAULT_FILE':' && ./build/$DEFAULT_FILE";
    crun.exec = "compile && runit";
    runtest.exec = "compile && for i in \$(seq 1 10); do runit; sleep 1; done";
  };
}
