tailwind.config = {
    darkMode: 'media',
    theme: {
        darkMode: 'media',
        fontFamily: {
            sans: ['Inter var', 'sans-serif'],
        },
        extend: {                        
            typography: ({ theme }) => ({
                DEFAULT: {
                    css: {
                        '--tw-prose-invert-pre-bg': '#1e293b',                        
                        '.prose :where(a):not(:where([class~=not-prose] *))': {
                            'color': 'var(--tw-prose-links)',
                            'text-decoration': 'none',
                            'font-weight': '600',
                            'border-bottom': '1px solid #7dd3fc'
                        },
                        '.prose :where(a:hover):not(:where([class~=not-prose] *))': {
                            'border-bottom-width': '2px'
                        },
                        '.prose :where(pre):not(:where([class~=not-prose] *))': {
                            'color': '#f8fafc',
                            'background-color': 'var(--tw-prose-pre-bg)',
                            'overflow-x': 'auto',
                            'font-weight': '400',
                            'font-size': '0.815em',
                            'line-height': '1.7142857',
                            'margin-top': '1.4285714285714286em',
                            'margin-bottom': '2.2857142857142856em',
                            'border-radius': '.75rem',
                            'padding': '1.25rem',
                            'box-shadow': 'inset 0 0 0 1px hsl(0deg 0% 100% / 10%),0 4px 6px -1px rgba(0,0,0,.1),0 2px 4px -2px rgba(0,0,0,.1)',
                            'display': 'flex'
                        }
                    }
                }
            })
        }
    }
}

/**
 * Once the window is loaded, locate the mobile menu button and add a click 
 * event listener to it. 
 *
 * This will toggle the hidden class on the mobile menu, which will show or
 * hide the menu in responsive, smaller screen sizes.
 */
window.onload = () => {
    
    let menu = document.getElementById('nav-menu');
    let menuOpen = document.getElementById('nav-button');
    let menuClose = document.getElementById('nav-menu-close');

    if (menuOpen && menu) {
        menuOpen.addEventListener('click', () => {
            menu.classList.toggle('hidden');
        });
    }

    if (menuClose && menu) {
        menuClose.addEventListener('click', () => {
            menu.classList.toggle('hidden');
        });
    }
}

/**
 * In order to reduce individual HTML page sizes, a page can include a JSON
 * file that contains the navigation menu items. This function will create
 * the navigation menu items from the JSON file dynamically.
 */
const createNav = ({ relative_root_path, nav_groups = [] }) => {    

    let mobileNav = document.getElementById('mobile-nav');
    let desktopNav = document.getElementById('desktop-nav');

    /* create mobile nav that is hidden on larger screens */    
    nav_groups.forEach((item, itemIndex) => {
        let navItemHeading = item[0];
        let navItemChildren = item[1];        

        let navItem = document.createElement('li');
        if (itemIndex > 0)
            navItem.classList.add('mt-12', 'lg:mt-8');

        let h5 = document.createElement('h5');
        h5.classList.add(
            'mb-8', 'lg:mb-3', 'font-semibold', 'text-slate-900', 'dark:text-slate-200'
        );
        h5.innerHTML = navItemHeading;

        let navItemChildrenList = document.createElement('ul');
        navItemChildren.forEach(child => {         
            let href = child[0];
            let text = child[1];
            let active = child[2];
            
            let li = document.createElement('li');
            navItemChildrenList.classList.add(
                'space-y-6', 'lg:space-y-2', 'border-l', 'border-slate-100', 
                'dark:border-slate-700'
            );
            let anchor = document.createElement('a');
            if (active) {
                anchor.classList.add(
                    'block', 'border-l', 'pl-4', '-ml-px', 'text-sky-500',
                    'font-semibold', 'dark:text-sky-400'
                );                
            } else {
                anchor.classList.add(
                    'block', 'border-l', 'pl-4', '-ml-px', 'border-transparent', 
                    'hover:border-slate-400', 'dark:hover:border-slate-500', 
                    'text-slate-700', 'hover:text-slate-900', 'dark:text-slate-400', 
                    'dark:hover:text-slate-300'
                );
            }
            anchor.innerHTML = text;
            anchor.setAttribute('href', relative_root_path + href);

            li.appendChild(anchor);
            navItemChildrenList.appendChild(li);
        });        
        navItem.appendChild(h5);
        navItem.appendChild(navItemChildrenList);

        mobileNav.appendChild(navItem);
    });

    /* create desktop nav that is hidden on smaller screens */
    nav_groups.forEach((item, itemIndex) => {
        let navItemHeading = item[0];
        let navItemChildren = item[1];

        let navItem = document.createElement('li');        
        navItem.classList.add('mt-12', 'lg:mt-8');

        let h5 = document.createElement('h5');
        h5.classList.add(
            'mb-8', 'lg:mb-3', 'font-semibold', 'text-slate-900', 'dark:text-slate-200'
        );
        h5.innerHTML = navItemHeading;

        let navItemChildrenList = document.createElement('ul');
        navItemChildrenList.classList.add(
            'space-y-6', 'lg:space-y-2', 'border-l', 'border-slate-100', 
            'dark:border-slate-700'
        );
        
        navItemChildren.forEach(child => {              
            let href = child[0];
            let text = child[1];
            let active = child[2];

            let li = document.createElement('li');
            let anchor = document.createElement('a');
            if (active) {
                anchor.classList.add(
                    'block', 'border-l', 'pl-4', '-ml-px', 'text-sky-500',
                    'font-semibold', 'dark:text-sky-400'
                ); 
            } else {
                anchor.classList.add(
                    'block', 'border-l', 'pl-4', '-ml-px', 'border-transparent', 
                    'hover:border-slate-400', 'dark:hover:border-slate-500', 
                    'text-slate-700', 'hover:text-slate-900', 'dark:text-slate-400', 
                    'dark:hover:text-slate-300'
                );
            }
            anchor.innerHTML = text;            
            anchor.setAttribute('href', relative_root_path + href);

            li.appendChild(anchor);            
            navItemChildrenList.appendChild(li);        
        });        
        
        navItem.appendChild(h5);
        navItem.appendChild(navItemChildrenList);

        desktopNav.appendChild(navItem);
    });
}
