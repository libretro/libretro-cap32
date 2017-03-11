
        static float ratio[] = {0.25f, NK_UNDEFINED};
        float spacing_x = ctx->style.window.spacing.x;

        /* output path directory selector in the menubar */
        ctx->style.window.spacing.x = 0;
        nk_menubar_begin(ctx);
        {
            char *d = browser->directory;
            char *begin = d + 1;
            nk_layout_row_dynamic(ctx, 16, 6);

            while (*d++) {
                if (*d == '/') {
                    *d = '\0';
                    if (nk_button_label(ctx, begin)) {
                        *d++ = '/'; *d = '\0';
                        file_browser_reload_directory_content(browser, browser->directory);
                        break;
                    }
                    *d = '/';
                    begin = d + 1;
                }
            }
        }
        nk_menubar_end(ctx);
        ctx->style.window.spacing.x = spacing_x;

        /* window layout */
        total_space = nk_window_get_content_region(ctx);
        nk_layout_row(ctx, NK_DYNAMIC, total_space.h, 2, ratio);
        nk_group_begin(ctx,"Special", NK_WINDOW_NO_SCROLLBAR);
        {

            nk_layout_row_dynamic(ctx, 16, 1);
            if (nk_button_label(ctx,  "Home"))
                file_browser_reload_directory_content(browser, browser->home);
            if (nk_button_label(ctx,"Desktop"))
                file_browser_reload_directory_content(browser, browser->desktop);
            if (nk_button_label(ctx,"/"))
                file_browser_reload_directory_content(browser, "/");
            if (nk_button_label(ctx,"Cancel"))
		LOADCONTENT=-1;
            nk_group_end(ctx);
        }

        /* output directory content window */
        nk_group_begin(ctx, "Content", 0);
        {
            int index = -1;
            size_t i = 0, j = 0, k = 0;
            size_t rows = 0, cols = 0;
            size_t count = browser->dir_count + browser->file_count;

            cols = 1;
            rows = count / cols;
            for (i = 0; i <= rows; i += 1) {

                {size_t n = j + cols;
                nk_layout_row_dynamic(ctx, 16, (int)cols);
                for (; j < count && j < n; ++j) {
                    /* draw one row of icons */
                    if (j < browser->dir_count) {
                        /* draw and execute directory buttons */
                        if (nk_button_label(ctx,browser->directories[j]))
                            index = (int)j;
                    } else {
                        /* draw and execute files buttons */
                       
                        size_t fileIndex = ((size_t)j - browser->dir_count);
                       
                        if (nk_button_label(ctx, browser->files[fileIndex])) {
                            strncpy(browser->file, browser->directory, MAX_PATH_LEN);
                            n = strlen(browser->file);
                            strncpy(browser->file + n, browser->files[fileIndex], MAX_PATH_LEN - n);
                            //ret = 1;
			    sprintf(LCONTENT,"%s",browser->file);
			    LOADCONTENT=2;
                        }
                    }
                }}

            }

            if (index != -1) {
                size_t n = strlen(browser->directory);
                strncpy(browser->directory + n, browser->directories[index], MAX_PATH_LEN - n);
                n = strlen(browser->directory);
                if (n < MAX_PATH_LEN - 1) {
                    browser->directory[n] = '/';
                    browser->directory[n+1] = '\0';
                }
                file_browser_reload_directory_content(browser, browser->directory);

            }
            nk_group_end(ctx);
        }
    

